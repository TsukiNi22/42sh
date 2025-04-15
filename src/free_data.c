/*
** EPITECH PROJECT, 2025
** free_data.c
** File description:
** Free data
*/

#include "hashtable.h"
#include "array.h"
#include "minishell.h"
#include "error.h"
#include <stdlib.h>
#include <unistd.h>

int free_array(char **array)
{
    if (!array)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (int i = 0; array[i]; i++)
        free(array[i]);
    free(array);
    return OK;
}

static int free_ptr(void *ptr)
{
    if (!ptr)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    free(ptr);
    return OK;
}

static int free_cmd(void *cmd)
{
    if (!cmd)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (delete_array((array_t **) &cmd, &free_ptr) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}

int free_input(void *input)
{
    if (!input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (delete_array((array_t **) &input, &free_cmd) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}

static void close_port(int stdin_save, int stdout_save)
{
    close(stdin_save);
    close(stdout_save);
}

static int clear_redirection_local(main_data_t *data)
{
    if (!data)
        return err_prog(PTR_ERR, EPITECH_ERR, ERR_INFO);
    for (int j = 0; j < 2; j++) {
        if (data->pipefd[j] != KO)
            close(data->pipefd[j]);
        data->pipefd[j] = KO;
    }
    if (dup2(data->stdin_save, STDIN_FILENO) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (dup2(data->stdout_save, STDOUT_FILENO) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}

static int free_prompt_string(main_data_t *data)
{
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    free(data->cmd_separator);
    free(data->conditional_string[0]);
    free(data->conditional_string[1]);
    for (int i = 0; i < 5; i++)
        free(data->redirection_string[i]);
    return delete_array(&(data->inputs), &free_cmd);
}

int free_data(main_data_t *data)
{
    if (!data)
        return err_prog(PTR_ERR, EPITECH_ERR, ERR_INFO);
    if (data->alias && data->env && data->sys_func) {
        delete_hashtable(data->alias, &free_hash_data_str);
        delete_hashtable(data->env, &free_hash_data_str);
        delete_hashtable(data->sys_func, &free_hash_data_str);
    }
    if (data->env_path)
        free_array(data->env_path);
    if (data->last_path && data->abs_path && data->relative_path
        && data->input && data->sys_mtime) {
        free(data->last_path);
        free(data->abs_path);
        free(data->relative_path);
        free(data->input);
        free(data->sys_mtime);
    }
    close_port(data->stdin_save, data->stdout_save);
    free_prompt_string(data);
    return clear_redirection_local(data);
}
