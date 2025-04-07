/*
** EPITECH PROJECT, 2025
** check_syntax_source.c
** File description:
** Check the syntax for env
*/

#include "my_string.h"
#include "minishell.h"
#include "error.h"

int check_syntax_source(main_data_t *data, array_t *input, int start)
{
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (input->len - start > 2)
        return err_system(data, OK, "source", "Too many arguments");
    if (input->len - start == 1)
        return OK;
    if (my_strcmp(input->data[start + 1], "-c") != 0
        && my_strcmp(input->data[start + 1], "--create") != 0)
        return err_system(data, OK, "source", "Invalid argument, can only be "
        "\'-c\' or \'--create\'");
    return OK;
}
