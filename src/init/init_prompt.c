/*
** EPITECH PROJECT, 2025
** init_promt.c
** File description:
** Init prompt
*/

#include "memory.h"
#include "minishell.h"
#include "error.h"

int init_prompt(main_data_t *data)
{
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    data->esc_char = '\\';
    data->cmd_separator = my_strdup(";");
    if (!data->cmd_separator)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    data->conditional_string[0] = my_strdup("&&");
    data->conditional_string[1] = my_strdup("||");
    if (!data->conditional_string[0] || !data->conditional_string[1])
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    data->redirection_string[0] = my_strdup("|");
    data->redirection_string[1] = my_strdup(">>");
    data->redirection_string[2] = my_strdup(">");
    data->redirection_string[3] = my_strdup("<<");
    data->redirection_string[4] = my_strdup("<");
    for (int i = 0; i < 5; i++) {
        if (!data->redirection_string[i])
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    return OK;
}
