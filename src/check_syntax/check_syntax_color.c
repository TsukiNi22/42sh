/*
** EPITECH PROJECT, 2025
** check_syntax_color.c
** File description:
** Check the syntax for color
*/

#include "write.h"
#include "my_string.h"
#include "minishell.h"
#include "error.h"
#include <stdlib.h>
#include <stddef.h>

int check_syntax_color(main_data_t *data, array_t *input, int start)
{
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (input->len - start > 4)
        return err_system(data, OK, "color", "Too many arguments");
    for (size_t i = 1; i < input->len - start; i++) {
        if (!is_valid_char(((char *) input->data[start + i])[0]))
            return err_system(data, OK, "color", "Expression Syntax");
        if (!my_str_isnumber(input->data[start + i]))
            return err_system(data, OK, "color", "Badly formed number");
    }
    return OK;
}
