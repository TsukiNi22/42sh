/*
** EPITECH PROJECT, 2025
** check_syntax.c
** File description:
** Check syntax fpr redirection / conditional
*/

#include "array.h"
#include "minishell.h"
#include "error.h"
#include <stddef.h>
#include <stdbool.h>

bool is_valid_char(char const c)
{
    if ((c < '0' || c > '9') && c != '-')
        return false;
    return true;
}

bool is_valid_str(char const *str)
{
    char c = '\0';

    if (!str)
        return err_prog(PTR_ERR, false, ERR_INFO);
    for (int i = 0; str[i]; i++) {
        c = str[i];
        if (!((c >= '0' && c <= '9') || c == '_'
            || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')))
            return false;
    }
    return true;
}

static bool is_empty(char *str)
{
    if (!str)
        return err_prog(PTR_ERR, true, ERR_INFO);
    for (int i = 0; str[i]; i++) {
        if (str[i] != ' ' && str[i] != '\t')
            return false;
    }
    return (!str[0]);
}

static int set_pipe(bool pipe[], array_t *inputs, int i)
{
    if (!pipe || !inputs)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    pipe[0] = (i > 0
    && *((int *) ((array_t *) inputs->data[i - 1])->data[0]) == -1
    && *((int *) ((array_t *) inputs->data[i - 1])->data[1]) == PIPE);
    pipe[1] = ((size_t) i + 1 < inputs->len
    && *((int *) ((array_t *) inputs->data[i + 1])->data[0]) == -1
    && *((int *) ((array_t *) inputs->data[i + 1])->data[1]) == PIPE);
    return OK;
}

static int check_syntax_redirection(main_data_t *data, array_t *input,
    array_t *inputs, int i)
{
    redirection_t red_val = PIPE;
    bool pipe[2] = {false};

    if (!data || !input || !inputs)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (set_pipe(pipe, inputs, i) == KO)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (int j = 0; j < *((int *) input->data[0]); j += 2) {
        if (is_empty(input->data[j + 2]))
            return err_system(data, OK, NULL, "Missing name for redirect");
        red_val = *((int *) input->data[j + 1]);
        if (pipe[0] && (red_val == D_LEFT || red_val == LEFT))
            return err_system(data, OK, NULL, "Ambiguous output redirect");
        if (pipe[1] && (red_val == D_RIGHT || red_val == RIGHT))
            return err_system(data, OK, NULL, "Ambiguous output redirect");
    }
    if (*((int *) input->data[0]) + 1 == (int) input->len)
        return err_system(data, OK, NULL, "Invalid null command");
    return OK;
}

static int check_syntax_input(main_data_t *data, array_t *input)
{
    int n = 0;

    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (size_t i = 0; i < input->len && !data->err_sys; i++) {
        n = *((int *) ((array_t *) input->data[i])->data[0]);
        if (n == -1 && (i + 1 >= input->len
            || *((int *) ((array_t *) input->data[i + 1])->data[0]) == -1))
            return err_system(data, OK, NULL, "Invalid null command");
        if (n > 0
            && check_syntax_redirection(data, input->data[i], input, i) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    return OK;
}

int check_syntax(main_data_t *data)
{
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (size_t i = 0; i < data->inputs->len && !data->err_sys; i++) {
        if (check_syntax_input(data, data->inputs->data[i]) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    return OK;
}
