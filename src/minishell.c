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

static void enable_raw_mode(main_data_t *data, struct termios *original)
{
    struct termios raw;

    tcgetattr(STDIN_FILENO, original);
    raw = *original;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    print_prompt(data, NULL);
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
    struct termios original;

    enable_raw_mode(data, &original);
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
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original);
    return OK;
}
