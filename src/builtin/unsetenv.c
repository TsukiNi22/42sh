/*
** EPITECH PROJECT, 2025
** unsetenv.c
** File description:
** Fucnction for the unsetenv builtin
*/

#include "my_string.h"
#include "minishell.h"
#include "error.h"
#include <stddef.h>

int builtin_unsetenv(main_data_t *data, array_t *input, int start)
{
    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (size_t i = 1; i < input->len - start; i++) {
        if (!ht_search(data->env, input->data[start + i]))
            continue;
        if (my_strcmp(input->data[start + i], "PATH") == 0)
            data->path_changed = true;
        if (ht_delete(data->env, input->data[start + i],
            &free_hash_data_str) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    return OK;
}
