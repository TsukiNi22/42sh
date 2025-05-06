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

static bool is_alias_char(char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        return true;
    return false;
}

static bool is_alias_str(char *str)
{
    char c = '\0';

    if (!str)
        return false;
    for (int i = 0; str[i]; i++) {
        c = str[i];
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
            || (c >= '0' && c <= '9') || c == '_'))
            return false;
    }
    return true;
}

static bool find_setup(main_data_t *data, char *str, char **save)
{
    if (!data || !str || !save)
        return err_prog(PTR_ERR, false, ERR_INFO);
    if (!ht_search(data->alias, str)) {
        free(str);
        return false;
    }
    if (*save)
        free(*save);
    *save = str;
    return true;
}

static int find_alias(main_data_t *data, char *input, char **save, int *size)
{
    char *str = NULL;
    bool end = false;
    int i = 0;

    if (!data || !input || !save || !size)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (i = 0; !end && input[i]; i++) {
        str = my_strndup(input, i + 1);
        if (!str)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        end = !is_alias_str(str);
        free(str);
    }
    if (end || !input[i]) {
        if (find_setup(data, my_strndup(input, i - end), save))
            *size = i - end;
    }
    return OK;
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
    count = my_strlen(data->input) +
    my_strlen(value) + my_strlen(&data->input[i] + size);
    if (my_malloc_c(&str, count + 1) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (!my_strcat(str, data->input) || !my_strcat(str, value)
        || !my_strcat(str, &data->input[i] + size))
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    free(data->input);
    data->input = str;
    return OK;
}

int replace_alias(main_data_t *data, bool *done)
{
    char *save = NULL;
    int size = KO;
    int res = OK;

    if (!data || !done)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (int i = 0; res == OK && data->input[i]; i++) {
        if ((i == 0 || data->input[i - 1] == ' ' || data->input[i + 1] == '\t')
            && is_alias_char(data->input[i])
            && find_alias(data, &data->input[i], &save, &size) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        if (save) {
            res += replace_alias_input(data, save, i, size);
            i += size;
            free(save);
            save = NULL;
        }
    }
    return KO * (res != OK);
}
