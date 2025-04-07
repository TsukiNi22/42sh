/*
** EPITECH PROJECT, 2025
** get_heredoc.c
** File description:
** Heredoc
*/

#include "define.h"
#include "memory.h"
#include "my_string.h"
#include "write.h"
#include "minishell.h"
#include "error.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

static int set_get_next_line(main_data_t *data, char **line)
{
    if (!data || !line)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (my_malloc_c(line, 1) == KO)
        return err_prog(MALLOC_ERR, KO, ERR_INFO);
    if (reset_ouput(STDOUT) == KO
        || strong(STDOUT) == KO
        || my_putstr(STDOUT, "heredoc> ") == KO
        || reset_ouput(STDOUT) == KO
        || color_rgb(STDOUT, data->r, data->g, data->b) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    *line[0] = '\0';
    return OK;
}

static char *get_next_line(main_data_t *data, bool *eof)
{
    char *line = NULL;
    char tmp[2] = {0};
    int res = 1;

    if (!data || !eof)
        return err_prog_n(PTR_ERR, ERR_INFO);
    if (set_get_next_line(data, &line) == KO)
        return err_prog_n(UNDEF_ERR, ERR_INFO);
    for (int i = 1; res == 1; i++) {
        res = read(STDIN_FILENO, tmp, 1);
        if (res == KO)
            return err_prog_n(UNDEF_ERR, ERR_INFO);
        if (tmp[0] == '\n')
            return line;
        if (my_realloc_c(&line, 1, i) == KO || !my_strcat(line, tmp))
            return err_prog_n(UNDEF_ERR, ERR_INFO);
        tmp[0] = '\0';
    }
    *eof = true;
    return line;
}

int get_heredoc(main_data_t *data, char *stop, int *pipefd)
{
    char *str = NULL;
    bool eof = false;

    if (!data || !stop || !pipefd)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    close(pipefd[0]);
    str = get_next_line(data, &eof);
    while (!eof && str && my_strcmp(str, stop) != 0) {
        if (my_putstr(pipefd[1], str) == KO
            || my_putchar(pipefd[1], '\n') == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        free(str);
        str = get_next_line(data, &eof);
    }
    if (!str || reset_ouput(STDIN_FILENO) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (str)
        free(str);
    close(pipefd[1]);
    return OK;
}
