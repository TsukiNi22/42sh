/*
** EPITECH PROJECT, 2025
** check_syntax_history.c
** File description:
** Check the syntax for history
*/

#include "my_string.h"
#include "minishell.h"
#include "error.h"

int check_syntax_history(main_data_t *data, array_t *input, int start)
{
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (input->len - start > 2)
        return err_system(data, OK, "history", "Too many arguments");
    if (input->len - start == 1)
        return OK;
    if (!my_str_isnum(input->data[start + 1]))
        return err_system(data, OK, "history", "The argument can only be "
        "composed of numeric character (0-9)");
    if (((char *) input->data[start + 1])[0] == '0')
        return err_system(data, OK, "history", "The argument can't start with "
        "\'0\'");
    return OK;
}
