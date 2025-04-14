/*
** EPITECH PROJECT, 2025
** minishell.c
** File description:
** Minishell
*/

#include "minishell.h"
#include "error.h"
#include <signal.h>
#include <stdbool.h>

static void set_signal(void)
{
    signal(SIGSEGV, handle_sigsegv);
    signal(SIGINT, handle_sigint);
    signal(SIGTSTP, handle_sigtstp);
    signal(SIGQUIT, handle_sigquit);
}

int do_input(main_data_t *data)
{
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (!data->out && replace_var(data) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (!data->out && inputs_parser(data) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (!data->out && check_syntax(data) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (!data->out && !data->err_sys && exe_inputs(data) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}

int minishell(main_data_t *data)
{
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (init_data(data) == KO)
        return err_custom("Data initialisation error", FATAL_ERR, ERR_INFO);
    set_signal();
    while (!data->out && !(data->input_redirect
        && data->return_value != OK)) {
        data->err_sys = false;
        if (get_input(data) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        if (!data->out)
            data->return_value = OK;
        if (!data->out && do_input(data) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    return OK;
}
