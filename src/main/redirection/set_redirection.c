/*
** EPITECH PROJECT, 2025
** set_redirect.c
** File description:
** Redirect < << > >> |
*/

#include "array.h"
#include "write.h"
#include "minishell.h"
#include "error.h"
#include <errno.h>
#include <string.h>
#include <wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

int clear_redirection(main_data_t *data, array_t *input, int i)
{
    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if ((size_t) i + 1 <= input->len - 1
        || ((size_t) i + 1 <= input->len - 1
        && *((int *) ((array_t *) input->data[i + 1])->data[1]) == PIPE))
        return OK;
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

static int redirect_left(main_data_t *data, char *file)
{
    int fd = OK;

    if (!data || !file)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    fd = open(file, O_RDONLY);
    if (fd == KO)
        return err_system(data, OK, file, strerror(errno));
    if (dup2(fd, STDIN_FILENO) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    close(fd);
    return OK;
}

static int redirect_d_left(main_data_t *data, char *stop)
{
    pid_t pid = OK;
    int pipefd[2] = {0};
    int status = OK;

    if (!data || !stop)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (pipe(pipefd))
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    pid = fork();
    if (pid == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (pid == OK)
        exit(get_heredoc(data, stop, pipefd));
    close(pipefd[1]);
    waitpid(pid, &status, WUNTRACED);
    if (dup2(pipefd[0], STDIN_FILENO) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    close(pipefd[0]);
    return WEXITSTATUS(status);
}

static int redirect_right(main_data_t *data, char *file)
{
    int fd = OK;

    if (!data || !file)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd == KO)
        return err_system(data, OK, file, strerror(errno));
    if (dup2(fd, STDOUT_FILENO) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    close(fd);
    return OK;
}

static int redirect_d_right(main_data_t *data, char *file)
{
    int fd = OK;

    if (!data || !file)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (fd == KO)
        return err_system(data, OK, file, strerror(errno));
    if (dup2(fd, STDOUT_FILENO) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    close(fd);
    return OK;
}

static int set_pipefd(main_data_t *data, array_t *input, int i)
{
    int type = OK;

    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if ((size_t) i + 1 > input->len - 1)
        return OK;
    type = *((int *) ((array_t *) input->data[i + 1])->data[1]);
    if (type == PIPE && pipe(data->pipefd) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}

int set_pipe_child(main_data_t *data)
{
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (data->prevfd != KO) {
        dup2(data->prevfd, STDIN_FILENO);
        close(data->prevfd);
        data->prevfd = KO;
    }
    if (data->pipefd[0] != KO && data->pipefd[1] != KO) {
        close(data->pipefd[0]);
        dup2(data->pipefd[1], STDOUT_FILENO);
        close(data->pipefd[1]);
    }
    return OK;
}

int set_pipe_parent(main_data_t *data, array_t *input, int i)
{
    int type = OK;

    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (data->prevfd != KO) {
        close(data->prevfd);
        data->prevfd = KO;
    }
    if ((size_t) i + 1 > input->len - 1)
        return OK;
    type = *((int *) ((array_t *) input->data[i + 1])->data[1]);
    if (type == PIPE && data->pipefd[0] != KO && data->pipefd[1] != KO) {
        close(data->pipefd[1]);
        data->pipefd[1] = KO;
        data->prevfd = data->pipefd[0];
    }
    return OK;
}

int set_redirection(main_data_t *data, array_t *input, array_t *inputs, int i)
{
    redirection_t red = PIPE;

    if (!data || !input || !inputs)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (set_pipefd(data, inputs, i) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    for (int j = 1; !data->err_sys && j < *((int *) input->data[0]); j += 2) {
        red = *((int *) input->data[j]);
        if (red == RIGHT && redirect_right(data, input->data[j + 1]) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        if (red == D_RIGHT && redirect_d_right(data, input->data[j + 1]) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        if (red == LEFT && redirect_left(data, input->data[j + 1]) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        if (red == D_LEFT && redirect_d_left(data, input->data[j + 1]) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    data->return_value = 1 * data->err_sys;
    return OK;
}
