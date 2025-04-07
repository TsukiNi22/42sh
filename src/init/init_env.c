/*
** EPITECH PROJECT, 2025
** init_env.c
** File description:
** Init env
*/

#include "memory.h"
#include "define.h"
#include "minishell.h"
#include "error.h"
#include <unistd.h>
#include <stdbool.h>

int init_env(main_data_t *data)
{
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    data->path_changed = false;
    data->sys_mtime = NULL;
    data->nb_mtime = KO;
    data->env_path = NULL;
    data->sys_func = NULL;
    data->last_path = my_strdup("");
    data->abs_path = getcwd(NULL, 0);
    if (!data->last_path || sys_func_update(data) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}
