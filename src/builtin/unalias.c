/*
** EPITECH PROJECT, 2025
** unalias.c
** File description:
** Fucnction for the unalias
*/

#include "minishell.h"
#include "error.h"
#include <stddef.h>

int builtin_unalias(main_data_t *data, array_t *input, int start)
{
    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (size_t i = 1; i < input->len - start; i++) {
        if (!ht_search(data->alias, input->data[start + i]))
            continue;
        if (ht_delete(data->alias, input->data[start + i],
            &free_hash_data_str) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    return OK;
}
