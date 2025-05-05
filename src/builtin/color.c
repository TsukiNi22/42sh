/*
** EPITECH PROJECT, 2025
** color.c
** File description:
** Builtin function color
*/

#include "define.h"
#include "write.h"
#include "my_string.h"
#include "minishell.h"
#include "error.h"
#include <pty.h>
#include <sys/wait.h>

static int pty_color(main_data_t *data)
{
    pid_t pid = OK;
    int status = 0;

    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    pid = forkpty(&data->master_fd, NULL, NULL, NULL);
    if (pid == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (pid == OK) {
        if (my_printf("Color set to: %d, %d, %d\n%C"
            "This is a color test :)\n%R",
            data->r, data->g, data->b, data->r, data->g, data->b) == KO)
            _exit(KO);
        _exit(OK);
    }
    if (pty_exec_handling(data, pid) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    waitpid(pid, &status, WUNTRACED);
    return WEXITSTATUS(status);
}

int builtin_color(main_data_t *data, array_t *input, int start)
{
    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    data->r = 255;
    data->g = 255;
    data->b = 255;
    if (input->len - start >= 2)
        data->r = my_atoi(input->data[start + 1]);
    if (input->len - start >= 3)
        data->g = my_atoi(input->data[start + 2]);
    if (input->len - start == 4)
        data->b = my_atoi(input->data[start + 3]);
    if (!data->silent && data->pty && pty_color(data) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (!data->silent && !data->pty &&
        my_printf("Color set to: %d, %d, %d\n%CThis is a color test :)\n%R",
        data->r, data->g, data->b, data->r, data->g, data->b) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}
