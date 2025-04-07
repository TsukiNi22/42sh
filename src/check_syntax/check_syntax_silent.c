/*
** EPITECH PROJECT, 2025
** check_syntax_silent.c
** File description:
** Check the syntax for .SILENT
*/

#include "my_string.h"
#include "minishell.h"
#include "error.h"

int check_syntax_silent(main_data_t *data, array_t *input, int start)
{
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (input->len - start > 2)
        return err_system(data, OK, ".SILENT", "Too many arguments");
    if (input->len - start > 1
        && my_strcmp(input->data[start + 1], "true") != 0
        && my_strcmp(input->data[start + 1], "false") != 0)
        return err_system(data, OK, ".SILENT", "Argument can only be "
        "'true' or 'false'");
    return OK;
}
