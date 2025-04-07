/*
** EPITECH PROJECT, 2025
** rehash.c
** File description:
** Fucnction for the rehash builtin
*/

#include "macro.h"
#include "write.h"
#include "define.h"
#include "hashtable.h"
#include "minishell.h"
#include "error.h"

int builtin_rehash(main_data_t *data, array_t *input, UNUSED int start)
{
    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (int i = 0; data->env_path[i]; i++)
        data->sys_mtime[i] = 0;
    if (sys_func_update(data) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}
