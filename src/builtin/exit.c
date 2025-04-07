/*
** EPITECH PROJECT, 2025
** exit.c
** File description:
** Builtin exit function
*/

#include "define.h"
#include "memory.h"
#include "my_string.h"
#include "write.h"
#include "minishell.h"
#include "error.h"
#include <stdlib.h>

int builtin_exit(main_data_t *data, array_t *input, int start)
{
    int n = OK;

    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (input->len - start == 2)
        n = my_atoi(input->data[start + 1]);
    if (free_data(data) == EPITECH_ERR)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (my_putstr(STDOUT, "exit\n") == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    exit(n);
}
