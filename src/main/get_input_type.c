/*
** EPITECH PROJECT, 2025
** get_input_type.c
** File description:
** Get the input type
*/

#include "array.h"
#include "my_string.h"
#include "write.h"
#include "define.h"
#include "minishell.h"
#include "error.h"
#include <stdbool.h>
#include <unistd.h>

static int is_builtin(main_data_t *data, array_t *input)
{
    int start = 0;

    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    start = *((int *) input->data[0]) + 1;
    for (int i = 0; builtin_array_name[i]; i++) {
        if (my_strcmp(input->data[start], builtin_array_name[i]) == 0) {
            data->builtin = true;
            data->builtin_val = i;
            return OK;
        }
    }
    return OK;
}

static int is_system_bin(main_data_t *data, array_t *input)
{
    char *cmd = NULL;
    int start = 0;

    if (!data || !input)
        return err_prog(PTR_ERR, false, ERR_INFO);
    start = *((int *) input->data[0]) + 1;
    if (sys_func_update(data) == KO)
        return err_prog(UNDEF_ERR, false, ERR_INFO);
    cmd = input->data[start];
    if (access(cmd, F_OK) == OK) {
        data->binary = true;
        return true;
    }
    if (ht_search(data->sys_func, cmd))
        return true;
    data->return_value = 1;
    return false;
}

static int check_syntax_dispatch(main_data_t *data, array_t *input,
        builtin_func_t cmd)
{
    int start = 0;

    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    start = *((int *) input->data[0]) + 1;
    if (cmd == NONE)
        return err_system(data, OK, input->data[start],
        "Unknow builtin command");
    if (cmd >= BUILTIN_MIN && cmd <= BUILTIN_MAX)
        return builtin_check_syntax[cmd](data, input, start);
    return err_system(data, OK, input->data[start], "Unknow builtin command");
}

int get_input_type(main_data_t *data, array_t *input)
{
    int st = 0;

    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    st = *((int *) input->data[0]) + 1;
    data->binary = false;
    data->builtin = false;
    data->builtin_val = NONE;
    if (input->len == 0 || st < 0)
        return OK;
    if (is_builtin(data, input) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (!data->builtin) {
        if (!is_system_bin(data, input))
            return err_system(data, OK, input->data[st], "Command not found");
        return OK;
    }
    if (check_syntax_dispatch(data, input, data->builtin_val) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    data->return_value = 1 * data->err_sys;
    return OK;
}
