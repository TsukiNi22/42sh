/*
** EPITECH PROJECT, 2025
** check_syntax_cd.c
** File description:
** Check the syntax for cd
*/

#include "my_string.h"
#include "write.h"
#include "memory.h"
#include "minishell.h"
#include "error.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

static int check_cd_file(main_data_t *data, char *path)
{
    struct stat st = {0};

    if (!data || !path)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (access(path, F_OK) == KO)
        return err_system(data, OK, path, "No such file or directory");
    if (stat(path, &st) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (!S_ISDIR(st.st_mode))
        return err_system(data, OK, path, "Not a directory");
    if (access(path, X_OK) == KO)
        return err_system(data, OK, path, "Permission denied");
    return OK;
}

int check_syntax_cd(main_data_t *data, array_t *input, int start)
{
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (input->len - start > 2)
        return err_system(data, OK, "cd", "Too many arguments");
    if (input->len - start == 1)
        return OK;
    if (my_strcmp(input->data[start + 1], "-") == 0)
        return check_cd_file(data, data->last_path);
    if (check_cd_file(data, input->data[start + 1]) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}
