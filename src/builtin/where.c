/*
** EPITECH PROJECT, 2025
** where.c
** File description:
** Builtin function where
*/

#include "hashtable.h"
#include "define.h"
#include "write.h"
#include "minishell.h"
#include "error.h"

static int pty_where(main_data_t *data)
{
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    return OK;
}

static int display_path(char *cmd)
{
    if (!cmd)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    return OK;
}

int builtin_where(main_data_t *data, array_t *input, int start)
{
    char *value = NULL;

    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (size_t i = 1; i < input->len - start; i++) {
        value = ht_search(data->sys_func, input->data[start + i]);
        if (!value)
            err_system(data, OK, input->data[start + i], "Was not found");
        if (value && display_path(input->data[start + i]) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    return OK;
}
