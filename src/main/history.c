/*
** EPITECH PROJECT, 2025
** history.c
** File description:
** Handle history
*/

#include "hashtable.h"
#include "write.h"
#include "file.h"
#include "my_string.h"
#include "minishell.h"
#include "error.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

static int create_file(char *path)
{
    int fd = 0;

    if (!path)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    fd = open(path, O_CREAT, 0600);
    if (fd == KO)
        return err_prog(OP_FILE_ERR, KO, ERR_INFO);
    close(fd);
    return OK;
}

static bool check_last(char *input, char *file)
{
    char *line = NULL;
    char c = '\0';
    int i = 0;

    if (!input || !file)
        return err_prog(PTR_ERR, false, ERR_INFO);
    for (i = 0; file[i] && file[i + 1]; i++) {
        if (file[i] == '\n')
            line = &(file[i + 1]);
    }
    if (!line)
        return false;
    for (i = 0; line[i] && line[i] != '\n'; i++);
    c = line[i];
    line[i] *= (line[i] != '\n');
    if (my_strcmp(input, line) == 0)
        return true;
    line[i] = c;
    return false;
}

static int del_line(char **file, char **tmp, char *nb)
{
    int line_nb = 0;
    int size = HISTORY_SIZE;

    if (!file || !(*file) || !tmp)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    *tmp = *file;
    if (nb && my_str_isnumber(nb))
        size = my_atoi(nb);
    line_nb = count_occurence(*file, '\n');
    for (; line_nb > 1 && line_nb >= size; line_nb--) {
        *file = my_strstr(*file, "\n");
        if (!(*file))
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        (*file)++;
    }
    return OK;
}

static int write_file(char *input, char *path, char *file)
{
    int fd = 0;

    if (!input || !file)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    fd = open(path, O_WRONLY | O_TRUNC);
    if (fd == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (my_printf("%O%s%s\n", fd, file, input) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    close(fd);
    return OK;
}

int add_history(main_data_t *data, hashtable_t *env, char *input)
{
    char *path = NULL;
    char *file = NULL;
    char *tmp = NULL;

    if (!data || !env || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    path = get_full_path(ht_search(env, "HOME"), HISTORY_FILE);
    if (data->input_redirect || !path)
        return OK;
    if (create_file(path) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    file = get_file(path);
    if (!file || check_last(input, file))
        return OK;
    if (del_line(&file, &tmp, ht_search(env, "HISTSIZE")) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (write_file(input, path, file) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    free(tmp);
    return OK;
}
