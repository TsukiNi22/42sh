/*
** EPITECH PROJECT, 2025
** help.c
** File description:
** Builtin function help
*/

#include "macro.h"
#include "define.h"
#include "write.h"
#include "minishell.h"
#include "error.h"

int builtin_help(main_data_t *data, array_t *input, UNUSED int start)
{
    int res = OK;

    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (int i = 0; help_ligne[i]; i++)
        res += my_putstr(STDOUT, help_ligne[i]);
    return res;
}
