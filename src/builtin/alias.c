/*
** EPITECH PROJECT, 2025
** alias.c
** File description:
** Fucnction for the env builtin
*/

#include "memory.h"
#include "write.h"
#include "define.h"
#include "hashtable.h"
#include "minishell.h"
#include "error.h"

static int display_alias(main_data_t *data)
{
    char **keys = NULL;
    char *value = NULL;

    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    keys = ht_keys(data->alias);
    if (!keys)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    for (int i = 0; keys[i]; i++) {
        value = ht_search(data->alias, keys[i]);
        if (!value || my_printf("%s='%s'\n", keys[i], value) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    if (free_hash_keys(keys) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    data->return_value = OK;
    return OK;
}

static int display_one_alias(main_data_t *data, array_t *input, int start)
{
    char *value = NULL;

    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    value = ht_search(data->alias, input->data[start + 1]);
    if (!value)
        return err_system(data, OK, input->data[start + 1], "Unknow alias");
    if (my_printf("%s='%s'\n", input->data[start + 1], value) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    data->return_value = OK;
    return OK;
}

int builtin_alias(main_data_t *data, array_t *input, int start)
{
    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    data->return_value = 1;
    if (input->len - start == 1)
        return display_alias(data);
    if (input->len - start == 2)
        return display_one_alias(data, input, start);
    if (ht_insert(data->alias, my_strdup(input->data[start + 1]),
        my_strdup(input->data[start + 2]), &free_hash_data_str) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (!data->silent &&
        my_printf("%s: Suceffuly added to alias for '%s'.\n",
        input->data[start + 1], input->data[start + 2]) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    data->return_value = OK;
    return OK;
}
