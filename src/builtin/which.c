/*
** EPITECH PROJECT, 2025
** which.c
** File description:
** Builtin function which
*/

#include "my_string.h"
#include "hashtable.h"
#include "define.h"
#include "write.h"
#include "minishell.h"
#include "error.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

static void free_path(char ***array)
{
    if (!array || !(*array))
        return;
    for (int i = 0; (*array)[i]; i++)
        free((*array)[i]);
    free(*array);
    *array = NULL;
}

static bool is_valid(char const *cmd_p)
{
    struct stat st = {0};

    if (!cmd_p)
        return err_prog(PTR_ERR, false, ERR_INFO);
    if (lstat(cmd_p, &st) == KO)
        return false;
    if (!S_ISREG(st.st_mode))
        return false;
    if (access(cmd_p, R_OK) == KO)
        return false;
    return true;
}

static int display_path(main_data_t *data, char *cmd, char *path)
{
    char *cmd_p = NULL;

    if (!data || !cmd)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (!path || my_strcmp(path, "") == 0) {
        if (my_putstr(STDERR, "No PATH found, automaticly use '/bin'\n") == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        path = "/bin";
    }
    free_path(&(data->env_path));
    data->env_path = str_to_str_array(path, ":", false);
    for (int i = 0; data->env_path[i]; i++) {
        if (!is_accesible_dir(data->env_path[i]))
            continue;
        cmd_p = get_full_path(data->env_path[i], cmd);
        if (is_valid(cmd_p))
            return my_printf("%s\n", cmd_p);
        free(cmd_p);
    }
    return OK;
}

static bool is_builtin(char *cmd)
{
    if (!cmd)
        return err_prog(PTR_ERR, false, ERR_INFO);
    for (int i = 0; builtin_array_name[i]; i++) {
        if (my_strcmp(builtin_array_name[i], cmd) == 0)
            return true;
    }
    return false;
}

static int display_which(main_data_t *data, char *cmd,
    char *var[2], bool builtin)
{
    if (!data || !cmd || !var)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (!var[0] && !var[1] && !builtin)
        return err_system(data, OK, cmd, "Was not found");
    if (var[0] && !var[1] && !builtin
        && display_path(data, cmd, ht_search(data->env, "PATH")) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (var[1] && !builtin
        && my_printf("%s='%s'\n", cmd, var[1]) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (builtin
        && my_printf("%s: Builtin\n", cmd) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}

int builtin_which(main_data_t *data, array_t *input, int start)
{
    char *value = NULL;
    char *alias = NULL;
    bool builtin = false;

    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (size_t i = 1; i < input->len - start; i++) {
        value = ht_search(data->sys_func, input->data[start + i]);
        alias = ht_search(data->alias, input->data[start + i]);
        builtin = is_builtin(input->data[start + i]);
        if (display_which(data, input->data[start + i],
            (char *[2]){value, alias}, builtin) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    return OK;
}
