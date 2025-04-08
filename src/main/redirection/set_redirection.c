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

int clear_redirection(main_data_t *data)
{
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
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

int set_redirection(main_data_t *data, array_t *input)
{
    redirection_t red = PIPE;

    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (int i = 1; !data->err_sys && i < *((int *) input->data[0]); i += 2) {
        red = *((int *) input->data[i]);
        if (red == RIGHT && redirect_right(data, input->data[i + 1]) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        if (red == D_RIGHT && redirect_d_right(data, input->data[i + 1]) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        if (red == LEFT && redirect_left(data, input->data[i + 1]) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        if (red == D_LEFT && redirect_d_left(data, input->data[i + 1]) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    data->return_value = 1 * data->err_sys;
    return OK;
}
