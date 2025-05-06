/*
** EPITECH PROJECT, 2025
** replace_alias.c
** File description:
** replace alias in input
*/

#include "minishell.h"
#include "error.h"

int replace_alias(main_data_t *data, bool *done)
{
    int size = -1;

    if (!data || !done)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (int i = 0; data->input[i]; i++) {
        for (int j = 0; data->input[i + j]; j++) {
            if ()
                size = j;
        }
    }
    return OK;
}
