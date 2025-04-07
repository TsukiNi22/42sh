/*
** EPITECH PROJECT, 2025
** silent.c
** File description:
** Builtin function silent
*/

#include "write.h"
#include "my_string.h"
#include "minishell.h"
#include "error.h"

int builtin_silent(main_data_t *data, array_t *input, int start)
{
    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    data->silent = !data->silent;
    if (input->len - start == 2) {
        if (((char *) input->data[start + 1])[0] == 't')
            data->silent = true;
        else
            data->silent = false;
    }
    return OK;
}
