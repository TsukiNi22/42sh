/*
** EPITECH PROJECT, 2025
** replace_alias.c
** File description:
** replace alias in input
*/

#include "memory.h"
#include "my_string.h"
#include "hashtable.h"
#include "minishell.h"
#include "error.h"
#include <stdlib.h>
#include <stdbool.h>

static int find_alias(main_data_t *data, char *input, char **save, int *size)
{
    char *str = NULL;
    bool end = false;
    int i = 0;

    if (!data || !input || !save || !size)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (i = 0; !end && input[i]; i++) {
        str = my_strndup(input, i + 1);
        end = !is_valid_str(str);
        free(str);
    }
    if (end || !input[i]) {
        str = my_strndup(input, i);
        if (!ht_search(data->alias, str)) {
            free(str);
            return OK;
        }
        if (*save)
            free(*save);
        *save = str;
        *size = i;
    }
    return OK;
}

static bool is_alias_char(char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        return true;
    return false;
}

static int replace_alias_input(main_data_t *data, char *save, int i, int size)
{
    char *value = NULL;
    char *str = NULL;
    int count = 0;

    if (!data || !save)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    value = ht_search(data->alias, save);
    if (!value)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    data->input[i] = '\0';
    count = my_strlen(data->input);
    count += my_strlen(value);
    count += my_strlen(data->input + size);
    if (my_malloc_c(&str, count + 1) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (!my_strcat(str, data->input) || !my_strcat(str, value)
        || !my_strcat(str, data->input + size))
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    free(data->input);
    data->input = str;
    return OK;
}

int replace_alias(main_data_t *data, bool *done)
{
    char *save = NULL;
    int size = KO;

    if (!data || !done)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (int i = 0; data->input[i]; i++) {
        if ((i == 0 || (!is_alias_char(data->input[i - 1])
            && data->input[i - 1] != data->esc_char)) && is_alias_char(data->input[i])
            && find_alias(data, &data->input[i], &save, &size) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        if (save) {
            if (replace_alias_input(data, save, i, size) == KO)
                return err_prog(UNDEF_ERR, KO, ERR_INFO);
            i += size;
            free(save);
            save = NULL;
        }
    }
    return OK;
}
