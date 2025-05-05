/*
** EPITECH PROJECT, 2025
** init_data.c
** File description:
** Init the main data
*/

#include "memory.h"
#include "define.h"
#include "minishell.h"
#include "error.h"
#include <stdlib.h>

static int basic_exe(main_data_t *data)
{
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    data->input = my_strdup("source");
    if (!data->input)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (do_input(data) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}

int init_data(main_data_t *data)
{
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (init_global(data) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (init_prompt(data) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (init_env(data) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (init_bonus(data) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (!data->input_redirect && basic_exe(data) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}
