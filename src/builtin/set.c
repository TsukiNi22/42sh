/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** set
*/

#include "minishell.h"
#include "error.h"
#include "printf.h"
#include "memory.h"
#include <pty.h>
#include <sys/wait.h>

static int pty_var(main_data_t *data, char **keys, char *value)
{
    pid_t pid = OK;
    int status = 0;
    int res = OK;

    if (!data || !keys)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    pid = forkpty(&data->master_fd, NULL, NULL, NULL);
    if (pid == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (pid == OK) {
        for (int i = 0; keys[i]; i++) {
            value = ht_search(data->var, keys[i]);
            res += KO * (!value || my_printf("%s=%s\n", keys[i], value) == KO);
        }
        _exit(KO * (res != OK));
    }
    if (pty_exec_handling(data, pid) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    waitpid(pid, &status, WUNTRACED);
    return WEXITSTATUS(status);
}

static int display_var(main_data_t *data)
{
    char **keys = NULL;
    char *value = NULL;

    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    keys = ht_keys(data->var);
    if (!keys)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (data->pty && pty_var(data, keys, value) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    for (int i = 0; !data->pty && keys[i]; i++) {
        value = ht_search(data->var, keys[i]);
        if (!value)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        if (my_printf("%s=%s\n", keys[i], value) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    if (free_hash_keys(keys) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}

int builtin_set(main_data_t *data, array_t *input, int start)
{
    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (input->len - start == 1) {
        data->return_value = OK;
        return display_var(data);
    }
    if (input->len - start == 3) {
        if (ht_insert(data->var, my_strdup(input->data[start + 1]),
            my_strdup(input->data[start + 2]), &free_hash_data_str) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    if (input->len - start == 2) {
        if (ht_insert(data->var, my_strdup(input->data[start + 1]),
            my_strdup(""), &free_hash_data_str) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    return OK;
}
