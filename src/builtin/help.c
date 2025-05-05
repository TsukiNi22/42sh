/*
** EPITECH PROJECT, 2025
** help.c
** File description:
** Builtin function help
*/

#include "macro.h"
#include "define.h"
#include "write.h"
#include "minishell.h"
#include "error.h"
#include <pty.h>
#include <sys/wait.h>

static int pty_help(main_data_t *data)
{
    pid_t pid = OK;
    int status = 0;
    int res = OK;

    if (!data)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    pid = forkpty(&data->master_fd, NULL, NULL, NULL);
    if (pid == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (pid == OK) {
        for (int i = 0; help_ligne[i]; i++)
            res += my_putstr(STDOUT, help_ligne[i]);
        if (res != OK)
            _exit(KO);
        _exit(OK);
    }
    if (pty_exec_handling(data, pid) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    waitpid(pid, &status, WUNTRACED);
    return WEXITSTATUS(status);
}

int builtin_help(main_data_t *data, array_t *input, UNUSED int start)
{
    int res = OK;

    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (data->pty && pty_help(data) == KO)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (int i = 0; !data->pty && help_ligne[i]; i++)
        res += my_putstr(STDOUT, help_ligne[i]);
    if (res != OK)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    return OK;
}
