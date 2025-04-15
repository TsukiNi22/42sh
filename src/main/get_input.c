/*
** EPITECH PROJECT, 2025
** get_input.c
** File description:
** Get the user input
*/

#include "define.h"
#include "write.h"
#include "my_string.h"
#include "minishell.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int get_input(main_data_t *data)
{
    int res = 0;

    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    free(data->input);
    if (data->inputs && delete_array(&(data->inputs), &free_input) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (!data->input_redirect && set_prompt(data) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    res = getline(&(data->input), &(size_t){0}, stdin);
    if (res == KO) {
        data->out = true;
        if (!data->input_redirect)
            my_putstr(STDOUT, "exit\n");
        return OK;
    }
    data->input[res - 1] = '\0';
    if (reset_ouput(1) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return add_history(data, data->env, data->input);
}
