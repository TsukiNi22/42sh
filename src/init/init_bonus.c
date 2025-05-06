/*
** EPITECH PROJECT, 2025
** init_bonus.c
** File description:
** Init bonus var
*/

#include "memory.h"
#include "hashtable.h"
#include "define.h"
#include "minishell.h"
#include "error.h"
#include <stdbool.h>

int init_bonus(main_data_t *data)
{
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    data->silent = false;
    data->alias = new_hashtable(&hash, DEFAULT_HASH_SIZE);
    data->var = new_hashtable(&hash, DEFAULT_HASH_SIZE);
    if (!data->alias || !data->var)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    data->relative_path = NULL;
    data->return_value = OK;
    data->r = 255;
    data->g = 255;
    data->b = 255;
    return OK;
}
