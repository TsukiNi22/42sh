/*
** EPITECH PROJECT, 2025
** history.c
** File description:
** Function for the history builtin
*/

#include "macro.h"
#include "my_math.h"
#include "my_string.h"
#include "write.h"
#include "file.h"
#include "define.h"
#include "minishell.h"
#include "error.h"
#include <stdlib.h>
#include <stdbool.h>
#include <pty.h>
#include <sys/wait.h>

static int pty_history(main_data_t *data, char **array)
{
    pid_t pid = OK;
    int status = 0;
    int res = OK;

    if (!data || !array)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    pid = forkpty(&data->master_fd, NULL, NULL, NULL);
    if (pid == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (pid == OK) {
        for (int i = 0; array[i]; i++)
            res += my_printf("%d: %s\n", i + 1, array[i]);
        if (res != OK)
            _exit(KO);
        _exit(OK);
    }
    if (pty_exec_handling(data, pid) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    waitpid(pid, &status, WUNTRACED);
    return WEXITSTATUS(status);
}

static int display(main_data_t *data,
    array_t *input, char *file, int line_nb)
{
    char **array = NULL;
    int display_nb = line_nb;
    int i = 0;

    if (!data || !input || !file)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (input->len - (*((int *) input->data[0]) + 1) == 2)
        display_nb = my_atoi(input->data[(*((int *) input->data[0]) + 1) + 1]);
    for (int size = 0; display_nb / my_pow(10, size) >= 1; size++);
    line_nb -= display_nb;
    for (i = 0; file[i] && line_nb > 0; i++)
        line_nb -= (file[i] == '\n');
    array = str_to_str_array(&(file[i]), "\n", false);
    if (!array || (data->pty && pty_history(data, array) == KO))
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    for (int j = 0; !data->pty && array[j]; j++) {
        if (my_printf("%d: %s\n", j + 1, array[j]) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    return free_array(array);
}

int builtin_history(main_data_t *data, array_t *input, UNUSED int start)
{
    char *file = NULL;
    char *path = NULL;
    int line_nb = 0;

    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    path = get_full_path(ht_search(data->env, "HOME"), HISTORY_FILE);
    data->return_value = 1 * (!path);
    if (!path)
        return err_system(data, OK, "history", "Can't found the HOME "
        "environement variable");
    file = get_file(path);
    line_nb = count_occurence(file, '\n');
    if (!file || line_nb < 0)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (display(data, input, file, line_nb) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    free(file);
    return OK;
}
