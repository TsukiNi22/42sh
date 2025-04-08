/*
** EPITECH PROJECT, 2025
** exe_input.c
** File description:
** Execute the user input for the no-builin function
*/

#include "define.h"
#include "write.h"
#include "memory.h"
#include "my_string.h"
#include "hashtable.h"
#include "minishell.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <errno.h>
#include <stddef.h>
#include <stdbool.h>

static int get_size(hashtable_t *env, char *key)
{
    char *value = NULL;
    int size = 0;
    int res = 0;

    if (!env || !key)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    value = ht_search(env, key);
    if (!value)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    res = my_strlen(value);
    if (res == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    size += res;
    res = my_strlen(key);
    if (res == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    size += res;
    return (size + 1);
}

static char **materialise_env(main_data_t *data)
{
    char **env = NULL;
    char **keys = NULL;

    if (!data)
        return err_prog_n(PTR_ERR, ERR_INFO);
    keys = ht_keys(data->env);
    if (!keys)
        return err_prog_n(UNDEF_ERR, ERR_INFO);
    env = malloc(sizeof(char *) * (data->env->keys_nbr + 1));
    if (!env)
        return err_prog_n(MALLOC_ERR, ERR_INFO);
    env[data->env->keys_nbr] = NULL;
    for (int i = 0; keys[i]; i++) {
        if (my_malloc_c(&env[i], get_size(data->env, keys[i]) + 1) == KO
            || !my_strcat(env[i], keys[i]) || !my_strcat(env[i], "=")
            || !my_strcat(env[i], ht_search(data->env, keys[i])))
            return err_prog_n(UNDEF_ERR, ERR_INFO);
    }
    free_hash_keys(keys);
    return env;
}

static char **materialise_cmd(array_t *input)
{
    char **cmd = NULL;
    int size = 0;

    if (!input)
        return err_prog_n(PTR_ERR, ERR_INFO);
    size = input->len - (*((int *) input->data[0]) + 1);
    cmd = malloc(sizeof(char *) * (size + 1));
    if (!cmd)
        return err_prog_n(MALLOC_ERR, ERR_INFO);
    cmd[size] = NULL;
    for (int i = 0; i < size; i++) {
        cmd[i] = my_strdup(input->data[*((int *) input->data[0]) + 1 + i]);
        if (!cmd[i])
            return err_prog_n(UNDEF_ERR, ERR_INFO);
    }
    return cmd;
}

static char *get_cmd_path(char **env_path, array_t *input, bool binary)
{
    char *cmd_path = NULL;
    char *cmd = NULL;

    if (!env_path || !input)
        return err_prog_n(PTR_ERR, ERR_INFO);
    cmd = input->data[*((int *) input->data[0]) + 1];
    if (binary)
        return my_strdup(cmd);
    for (int i = 0; env_path[i]; i++) {
        cmd_path = get_full_path(env_path[i], cmd);
        if (!cmd_path)
            return err_prog_n(UNDEF_ERR, ERR_INFO);
        if (access(cmd_path, X_OK) == OK)
            return cmd_path;
        free(cmd_path);
    }
    return err_system_n(NULL, cmd, "Can't find a path to execute");
}

static void clear_memory_error_exec(main_data_t *data,
    char **env, char **cmd, char *cmd_path)
{
    if (!data || !env || !cmd || !cmd_path)
        exit(EPITECH_ERR);
    free(cmd_path);
    free_array(env);
    free_array(cmd);
    free_data(data);
    exit(1 + 125 * (errno == ENOEXEC));
}

static void child(main_data_t *data, array_t *input)
{
    char **env = NULL;
    char **cmd = NULL;
    char *cmd_path = NULL;

    if (!data || !input)
        exit(EPITECH_ERR);
    env = materialise_env(data);
    cmd = materialise_cmd(input);
    cmd_path = get_cmd_path(data->env_path, input, data->binary);
    if (!env || !cmd || !cmd_path)
        exit(EPITECH_ERR);
    execve(cmd_path, cmd, env);
    if (errno == ENOEXEC)
        err_system(data, KO, input->data[*((int *) input->data[0]) + 1],
        "Exec format error. Binary file not executable");
    else
        err_system(data, KO, input->data[*((int *) input->data[0]) + 1],
        strerror(errno));
    clear_memory_error_exec(data, env, cmd, cmd_path);
}

static int handle_return(main_data_t *data, pid_t pid)
{
    int status = OK;
    int signal = 0;

    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    waitpid(pid, &status, WUNTRACED);
    if (WIFSIGNALED(status)) {
        signal = WTERMSIG(status);
        data->return_value = 128 + signal;
        if (signal == SIGFPE)
            return my_putstr(STDERR, "Floating exception (core dumped)\n");
        if (signal == SIGSEGV)
            return my_putstr(STDERR, "Segmentation fault (core dumped)\n");
    } else
        data->return_value = WEXITSTATUS(status);
    return OK;
}

int exe_cmd(main_data_t *data, array_t *cmd)
{
    pid_t pid = OK;

    if (!data || !cmd)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (get_input_type(data, cmd) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (data->err_sys)
        return OK;
    if (!data->builtin) {
        pid = fork();
        if (pid == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        if (pid == OK)
            child(data, cmd);
        if (handle_return(data, pid) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    } else if (builtin_func[data->builtin_val](data,
        cmd, *((int *) cmd->data[0]) + 1) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return clear_redirection(data);
}

int exe_input(main_data_t *data, array_t *input)
{
    int type = 0;
    int nb = 0;

    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (size_t i = 0; i < input->len; i++) {
        nb = *((int *) ((array_t *) input->data[i])->data[0]);
        if (nb < 0)
            type = *((int *) ((array_t *) input->data[i])->data[1]);
        if (nb < 0 && ((type == AND && data->return_value != OK)
            || (type == OR && data->return_value == OK)))
            break;
        if (nb < 0)
            continue;
        if (set_redirection(data, input->data[i]) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        if (exe_cmd(data, input->data[i]) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    return OK;
}

int exe_inputs(main_data_t *data)
{
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (size_t i = 0; i < data->inputs->len; i++) {
        if (exe_input(data, data->inputs->data[i]) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    return OK;
}
