/*
** EPITECH PROJECT, 2025
** check_syntax_alias.c
** File description:
** Check the syntax for alias
*/

#include "minishell.h"
#include "error.h"

int check_syntax_alias(main_data_t *data, array_t *input, int start)
{
    char c = '\0';

    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (input->len - start > 3)
        return err_system(data, OK, "alias", "Too many arguments");
    if (input->len - start < 2)
        return OK;
    c = ((char *) input->data[start + 1])[0];
    if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')))
        return err_system(data, OK, "alias", "Alias name "
        "must begin with a letter");
    if (!is_valid_str(input->data[start + 1]))
        return err_system(data, OK, "alias", "Alias name "
        "must contain alphanumeric characters");
    return OK;
}
