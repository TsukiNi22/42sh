/*
** EPITECH PROJECT, 2025
** source.c
** File description:
** Function for the source builtin
*/

#include "macro.h"
#include "memory.h"
#include "define.h"
#include "my_string.h"
#include "write.h"
#include "file.h"
#include "array.h"
#include "my_string.h"
#include "minishell.h"
#include "error.h"
#include <wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

static int exec_line(main_data_t *data, char *line)
{
    if (!data || !line)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    data->input = line;
    if (do_input(data) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    free(data->input);
    if (delete_array(&(data->inputs), &free_input) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}

static int exec_lines(main_data_t *data, char **lines, char *path)
{
    int j = 0;

    if (!data || !lines)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (int i = 0; lines[i]; i++) {
        if (lines[i][0] == '#')
            continue;
        for (j = 0; lines[i][j] && lines[i][j] != '#'; j++);
        lines[i][j] = '\0';
        if (exec_line(data, my_strdup(lines[i])) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        if (data->err_sys && !data->source_stoped
            && my_printf("%Oline %d: %C%s\n%R",
            STDERR, i, 255, 0, 0, lines[i]) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        if (data->err_sys && !data->source_stoped)
            return err_system(data, OK, path, "Syntax error");
    }
    return OK;
}

static int create_file(char *path)
{
    int res = OK;
    int fd = 0;

    if (!path)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    fd = open(path, O_CREAT | O_WRONLY, 00644);
    if (fd == KO)
        return err_prog(OP_FILE_ERR, KO, ERR_INFO);
    for (int i = 0; rc_ligne[i]; i++)
        res += my_putstr(fd, rc_ligne[i]);
    if (res != OK)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    close(fd);
    return OK;
}

static int set_file(main_data_t *data, array_t *input, char **file,
    char *path)
{
    if (!data || !input || !file || !path)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (!path) {
        data->return_value = 1;
        err_system(data, OK, "source", "Can't found the HOME "
        "environement variable");
        data->source_depth = 0;
        return KO;
    }
    *file = get_file(path);
    if (!(*file)) {
        if (input->len - (*((int *) input->data[0]) + 1) == 2)
            create_file(path);
        else
            err_system(data, OK, "source", "Create a file ~/.myshrc or "
            "use \'source -c\' if you want to use \'source\'");
        data->source_depth = 0;
        return KO;
    }
    return OK;
}

static int set_file_arg(char **file, char *path)
{
    if (!file || !path)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    *file = get_file(path);
    if (!(*file))
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}

static int exe_file(main_data_t *data, char **lines,
    array_t *input, int start)
{
    array_t *inputs_save = NULL;
    char *input_save = NULL;
    char *path = "~/.bananarc";

    if (!data || !lines || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (input->len - start == 2)
        path = input->data[start + 1];
    input_save = data->input;
    inputs_save = data->inputs;
    if (!lines || exec_lines(data, lines, path) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    data->input = input_save;
    data->inputs = inputs_save;
    free_array(lines);
    data->source_depth = 0;
    return OK;
}

int builtin_source(main_data_t *data, array_t *input, int start)
{
    char **lines = NULL;
    char *file = NULL;
    bool c = false;

    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    data->source_depth++;
    data->source_stoped = (data->source_depth >= 10);
    if (data->source_stoped)
        return err_system(data, OK, "source", "Stoped due to a depth >= 10");
    c = input->len - start == 1 || my_strcmp(input->data[start + 1], "-c") == 0
    || my_strcmp(input->data[start + 1], "--create") == 0;
    if (c && set_file(data, input, &file,
        get_full_path(ht_search(data->env, "HOME"), MYSHRC_FILE)) == KO)
        return OK;
    if (!c && set_file_arg(&file, input->data[start + 1]) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    lines = str_to_str_array(file, "\n", false);
    free(file);
    return exe_file(data, lines, input, start);
}
