/*
** EPITECH PROJECT, 2025
** setenv.c
** File description:
** Fucnction for the setenv builtin
*/

#include "my_string.h"
#include "memory.h"
#include "hashtable.h"
#include "minishell.h"
#include "error.h"
#include <stdbool.h>

int builtin_setenv(main_data_t *data, array_t *input, int start)
{
    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (input->len - start == 1) {
        data->return_value = OK;
        return builtin_env(data, input, start);
    }
    if (my_strcmp(input->data[start + 1], "PATH") == 0)
        data->path_changed = true;
    if (input->len - start == 2) {
        if (ht_insert(data->env, my_strdup(input->data[start + 1]),
            my_strdup(input->data[start + 2]), &free_hash_data_str) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    if (input->len - start != 1 && input->len - start != 2) {
        if (ht_insert(data->env, my_strdup(input->data[start + 1]),
            my_strdup(""), &free_hash_data_str) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    return OK;
}
