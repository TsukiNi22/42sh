/*
** EPITECH PROJECT, 2025
** env.c
** File description:
** Fucnction for the env builtin
*/

#include "macro.h"
#include "write.h"
#include "define.h"
#include "hashtable.h"
#include "minishell.h"
#include "error.h"
#include <pty.h>
#include <sys/wait.h>

static int pty_env(main_data_t *data, char **keys, char *value)
{
    pid_t pid = OK;
    int status = 0;

    if (!data || !keys)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    pid = forkpty(&data->master_fd, NULL, NULL, NULL);
    if (pid == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (pid == OK) {
        for (int i = 0; keys[i]; i++) {
            value = ht_search(data->env, keys[i]);
            if (!value || my_printf("%s=%s\n", keys[i], value) == KO)
                _exit(KO);
        }
        _exit(OK);
    }
    if (pty_exec_handling(data, pid) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    waitpid(pid, &status, WUNTRACED);
    return WEXITSTATUS(status);
}

int builtin_env(main_data_t *data, array_t *input, UNUSED int start)
{
    char **keys = NULL;
    char *value = NULL;

    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    keys = ht_keys(data->env);
    if (!keys)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (data->pty && pty_env(data, keys, value) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    for (int i = 0; !data->pty && keys[i]; i++) {
        value = ht_search(data->env, keys[i]);
        if (!value)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        if (my_printf("%s=%s\n", keys[i], value) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    if (free_hash_keys(keys) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}
