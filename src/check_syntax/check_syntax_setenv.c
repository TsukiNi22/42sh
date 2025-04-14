/*
** EPITECH PROJECT, 2025
** check_syntax_setenv.c
** File description:
** Check the syntax for setenv
*/

#include "minishell.h"
#include "error.h"

int check_syntax_setenv(main_data_t *data, array_t *input, int start)
{
    char c = '\0';

    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (input->len - start > 3)
        return err_system(data, OK, "setenv", "Too many arguments");
    if (input->len - start == 1)
        return OK;
    c = ((char *) input->data[start + 1])[0];
    if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) && c != '_')
        return err_system(data, OK, "setenv", "Variable name "
        "must begin with a letter");
    if (!is_valid_str(input->data[start + 1]))
        return err_system(data, OK, "setenv", "Variable name "
        "must contain alphanumeric characters");
    return OK;
}
