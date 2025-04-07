/*
** EPITECH PROJECT, 2025
** cd.c
** File description:
** Fucnction for the cd builtin
*/

#include "hashtable.h"
#include "memory.h"
#include "my_string.h"
#include "define.h"
#include "minishell.h"
#include "error.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

static char *get_home_path(main_data_t *data)
{
    char *path = NULL;

    if (!data)
        return err_prog_n(PTR_ERR, ERR_INFO);
    path = ht_search(data->env, "HOME");
    if (!path)
        return err_system_n(data, "cd", "Can't found the HOME "
        "environement variable\n");
    else if (access(path, F_OK) == KO)
        return err_system_n(data, "cd", "The HOME"
        "environement variable is invalid\n");
    return path;
}

static int special_case(main_data_t *data, array_t *input, int start)
{
    char *tmp = NULL;
    char *path = NULL;

    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (input->len - start == 1) {
        path = get_home_path(data);
        if (!data)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        data->last_path = data->abs_path;
        data->abs_path = path;
    } else if (((char *) input->data[start + 1])[0] == '-') {
        tmp = data->abs_path;
        data->abs_path = data->last_path;
        data->last_path = tmp;
    }
    if (chdir(data->abs_path) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    data->return_value = OK;
    return OK;
}

int builtin_cd(main_data_t *data, array_t *input, int start)
{
    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    data->return_value = 1;
    if (input->len - start == 1 || my_strcmp(input->data[start + 1], "-") == 0)
        return special_case(data, input, start);
    if (chdir(input->data[start + 1]) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    free(data->last_path);
    data->last_path = data->abs_path;
    data->abs_path = getcwd(NULL, 0);
    if (ht_insert(data->env, "OLDPWD", my_strdup(data->last_path),
        &free_hash_data_str) == KO
        || ht_insert(data->env, "PWD", my_strdup(data->abs_path),
        &free_hash_data_str) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    data->return_value = OK;
    return OK;
}
