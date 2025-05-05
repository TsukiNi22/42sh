/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** set
*/

#include "minishell.h"
#include "../../include/error.h"
#include "printf.h"
#include "memory.h"

static int display_var(main_data_t *data)
{
    char **keys = NULL;
    char *value = NULL;

    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    keys = ht_keys(data->var);
    if (!keys)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    for (int i = 0; keys[i]; i++) {
        value = ht_search(data->env, keys[i]);
        if (!value)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        if (my_printf("%s\t%s\n", keys[i], value) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    if (free_hash_keys(keys) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}

int builtin_set(main_data_t *data, array_t *input, int start)
{
    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (input->len - start == 1) {
        data->return_value = OK;
        return display_var(data);
    }
    if (input->len - start == 2) {
        if (ht_insert(data->var, my_strdup(input->data[start + 1]),
            my_strdup(input->data[start + 2]), &free_hash_data_str) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    if (input->len - start != 1 && input->len - start != 2) {
        if (ht_insert(data->var, my_strdup(input->data[start + 1]),
            my_strdup(""), &free_hash_data_str) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    return OK;
}
