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
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <SFML/Graphics.h>

void enable_raw_mode(main_data_t *data)
{
    struct termios raw;

    tcgetattr(STDIN_FILENO, &data->original);
    raw = data->original;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    fflush(stdout);
}

void set_signal(void)
{
    struct sigaction sa;

    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    signal(SIGSEGV, handle_sigsegv);
    signal(SIGTSTP, handle_sigtstp);
    signal(SIGQUIT, handle_sigquit);
}

int do_input(main_data_t *data)
{
    bool done = true;

    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (int i = 0; done && i < 10; i++) {
        done = false;
        if (!data->out && !data->err_sys && replace_goblin(data, &done) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        if (!data->out && !data->err_sys && replace_alias(data, &done) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        if (!data->out && !data->err_sys && replace_var(data, &done) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    if (!data->out && !data->err_sys && inputs_parser(data) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (!data->out && !data->err_sys && check_syntax(data) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (!data->out && !data->err_sys && exe_inputs(data) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}

int minishell(main_data_t *data)
{
    enable_raw_mode(data);
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (init_data(data) == KO)
        return err_custom("Data initialisation error", FATAL_ERR, ERR_INFO);
    set_signal();
    while ((!data->pty || sfRenderWindow_isOpen(data->terminal->window))
        && !data->out && !(data->input_redirect && data->return_value != OK)) {
        data->err_sys = false;
        if (get_input(data) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        if (!data->out)
            data->return_value = OK;
        if (!data->out && do_input(data) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &data->original);
    return OK;
}
