/*
** EPITECH PROJECT, 2025
** alias.c
** File description:
** Fucnction for the env builtin
*/

#include "memory.h"
#include "write.h"
#include "define.h"
#include "hashtable.h"
#include "minishell.h"
#include "error.h"
#include <pty.h>
#include <sys/wait.h>

static int pty_display_alias(main_data_t *data, char *key, char *value)
{
    pid_t pid = OK;
    int status = 0;

    if (!data || !key || !value)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    pid = forkpty(&data->master_fd, NULL, NULL, NULL);
    if (pid == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (pid == OK) {
        if (my_printf("%s='%s'\n", key, value) == KO)
            _exit(KO);
        _exit(OK);
    }
    if (pty_exec_handling(data, pid) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    waitpid(pid, &status, WUNTRACED);
    return WEXITSTATUS(status);
}

static int display_alias(main_data_t *data)
{
    char **keys = NULL;
    char *val = NULL;

    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    keys = ht_keys(data->alias);
    if (!keys)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    for (int i = 0; keys[i]; i++) {
        val = ht_search(data->alias, keys[i]);
        if (!val || (!data->pty && my_printf("%s='%s'\n", keys[i], val) < 0))
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        if (data->pty && pty_display_alias(data, keys[i], val) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    if (free_hash_keys(keys) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    data->return_value = OK;
    return OK;
}

static int pty_display_one_alias(main_data_t *data,
    array_t *input, int start, char *value)
{
    pid_t pid = OK;
    int status = 0;

    if (!data || !input || !value)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    pid = forkpty(&data->master_fd, NULL, NULL, NULL);
    if (pid == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (pid == OK) {
        if (my_printf("%s='%s'\n", input->data[start + 1], value) == KO)
            _exit(KO);
        _exit(OK);
    }
    if (pty_exec_handling(data, pid) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    waitpid(pid, &status, WUNTRACED);
    return WEXITSTATUS(status);
}

static int display_one_alias(main_data_t *data, array_t *input, int start)
{
    char *val = NULL;

    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    val = ht_search(data->alias, input->data[start + 1]);
    if (!val)
        return err_system(data, OK, input->data[start + 1], "Unknow alias");
    if (data->pty && pty_display_one_alias(data, input, start, val) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (!data->pty && my_printf("%s='%s'\n", input->data[start + 1], val) < 0)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    data->return_value = OK;
    return OK;
}

static int pty_alias(main_data_t *data, array_t *input, int start)
{
    pid_t pid = OK;
    int status = 0;

    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    pid = forkpty(&data->master_fd, NULL, NULL, NULL);
    if (pid == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (pid == OK) {
        if (my_printf("%s: Suceffuly added to alias for '%s'.\n",
            input->data[start + 1], input->data[start + 2]) == KO)
            _exit(KO);
        _exit(OK);
    }
    if (pty_exec_handling(data, pid) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    waitpid(pid, &status, WUNTRACED);
    return WEXITSTATUS(status);
}

int builtin_alias(main_data_t *data, array_t *input, int start)
{
    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    data->return_value = 1;
    if (input->len - start == 1)
        return display_alias(data);
    if (input->len - start == 2)
        return display_one_alias(data, input, start);
    if (ht_insert(data->alias, my_strdup(input->data[start + 1]),
        my_strdup(input->data[start + 2]), &free_hash_data_str) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (!data->silent && !data->pty &&
        my_printf("%s: Suceffuly added to alias for '%s'.\n",
        input->data[start + 1], input->data[start + 2]) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (!data->silent && data->pty && pty_alias(data, input, start) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    data->return_value = OK;
    return OK;
}
