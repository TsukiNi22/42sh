/*
** EPITECH PROJECT, 2025
** check_syntax_where.c
** File description:
** Check the syntax for where
*/

#include "minishell.h"
#include "error.h"

int check_syntax_where(main_data_t *data, array_t *input, int start)
{
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (input->len - start == 1)
        return err_system(data, OK, "where", "Too few arguments");
    return OK;
}
