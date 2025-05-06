/*
** EPITECH PROJECT, 2025
** clear.c
** File description:
** Builtin function clear
*/

#include "macro.h"
#include "define.h"
#include "write.h"
#include "minishell.h"
#include "error.h"
#include <string.h>

static int pty_clear(main_data_t *data)
{
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    memset(data->terminal->lines, 0, sizeof(char) * (MAX_LINES - 1) * MAX_LINE_LENGTH);
    memset(data->terminal->apartenance, 0, sizeof(int) * (MAX_LINES - 1));
    data->terminal->last_apartenance = -1;
    data->terminal->line_count = 0;
    data->terminal->actual_ligne = 0;
    return OK;
}

int builtin_clear(main_data_t *data, array_t *input, UNUSED int start)
{
    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (data->pty && pty_clear(data) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    else if (!data->pty && my_printf("%c[H%c[2J", ESC, ESC) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}
