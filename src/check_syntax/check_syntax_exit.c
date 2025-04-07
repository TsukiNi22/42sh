/*
** EPITECH PROJECT, 2025
** check_syntax_exit.c
** File description:
** Check the syntax for exit
*/

#include "write.h"
#include "my_string.h"
#include "minishell.h"
#include "error.h"
#include <stdlib.h>

int check_syntax_exit(main_data_t *data, array_t *input, int start)
{
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (input->len - start == 1)
        return OK;
    if (!is_valid_char(((char *) input->data[start + 1])[0]))
        return err_system(data, OK, "exit", "Expression Syntax");
    if (!my_str_isnumber(input->data[start + 1]))
        return err_system(data, OK, "exit", "Badly formed number");
    if (input->len - start > 2)
        return err_system(data, OK, "exit", "Expression Syntax");
    return OK;
}
