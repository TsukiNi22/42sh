/*
** EPITECH PROJECT, 2025
** init_global.c
** File description:
** Init struct for main data
*/

#include "define.h"
#include "minishell.h"
#include "error.h"
#include <unistd.h>
#include <stdbool.h>

int init_global(main_data_t *data)
{
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    data->out = false;
    data->binary = false;
    data->builtin = false;
    data->builtin_val = NONE;
    data->input_redirect = (!isatty(STDIN_FILENO) && !data->pty);
    data->err_sys = false;
    data->input = NULL;
    data->inputs = NULL;
    data->pipefd[0] = KO;
    data->pipefd[1] = KO;
    data->prevfd = KO;
    data->stdin_save = dup(STDIN_FILENO);
    data->stdout_save = dup(STDOUT_FILENO);
    if (data->stdin_save == KO || data->stdout_save == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}
