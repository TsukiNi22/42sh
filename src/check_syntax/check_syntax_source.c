/*
** EPITECH PROJECT, 2025
** check_syntax_source.c
** File description:
** Check the syntax for env
*/

#include "my_string.h"
#include "minishell.h"
#include "error.h"
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

static bool check_file(main_data_t *data, const char *path)
{
    struct stat st = {0};

    if (!data)
        return err_prog(PTR_ERR, false, ERR_INFO);
    if (lstat(path, &st) == KO)
        return err_system(data, false, path, strerror(errno));
    if (!S_ISREG(st.st_mode))
        return err_system(data, false, path, "Definitly not a regular file");
    if (access(path, R_OK) == KO)
        return err_system(data, false, path, strerror(errno));
    return true;
}

int check_syntax_source(main_data_t *data, array_t *input, int start)
{
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (input->len - start > 2)
        return err_system(data, OK, "source", "Too many arguments");
    if (input->len - start == 1)
        return OK;
    if (my_strcmp(input->data[start + 1], "-c") != 0
        && my_strcmp(input->data[start + 1], "--create") != 0
        && !check_file(data, input->data[start + 1]))
        data->err_sys = true;
    return OK;
}
