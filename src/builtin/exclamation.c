/*
** EPITECH PROJECT, 2025
** exclamation.c
** File description:
** function for the ! builtin
*/

#include "error.h"
#include "minishell.h"
#include "my_string.h"
#include "memory.h"
#include "file.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static char *read_file(main_data_t *data)
{
    char *path = NULL;
    char *file = NULL;

    if (!data)
        return NULL;
    path = get_full_path(ht_search(data->env, "HOME"), HISTORY_FILE);
    if (!path)
        return NULL;
    file = get_file(path);
    if (!file)
        return NULL;
    return file;
}

static char *get_start(char *start, int n)
{
    int line_count = 1;

    while (*start && line_count < n) {
        if (*start == '\n')
            line_count++;
        start++;
    }
    return start;
}

static char *correct_last(char *second_last_newline, char *last_newline)
{
    size_t line_len = 0;
    char *start = NULL;
    char *result = NULL;

    if (!second_last_newline || !last_newline)
        return NULL;
    start = second_last_newline + 1;
    line_len = last_newline - start;
    if (my_malloc_c(&result, line_len + 1) == KO)
        return NULL;
    strncpy(result, start, line_len);
    result[line_len] = '\0';
    return result;
}

static size_t get_len(char *file)
{
    size_t len = 0;

    if (!file)
        return 0;
    len = my_strlen(file);
    while (len > 0 && (file[len - 1] == '\n' || file[len - 1] == '\r'))
        len--;
    return len;
}

static char *execute_last(char *file)
{
    size_t len = 0;
    char *last_newline = NULL;
    char *second_last_newline = NULL;

    if (!file)
        return NULL;
    len = get_len(file);
    if (len == 0)
        return NULL;
    for (char *p = file + len - 1; p >= file; p--) {
        if (*p != '\n')
            continue;
        if (!last_newline) {
            last_newline = p;
            continue;
        }
        second_last_newline = p;
        break;
    }
    return correct_last(second_last_newline, last_newline);
}

static char *execute_n(char *file, int n)
{
    char *line = NULL;
    char *start = NULL;
    char *end = NULL;

    if (!file || n < 0)
        return NULL;
    start = file;
    start = get_start(start, n);
    if (!*start)
        return NULL;
    end = start;
    while (*end && *end != '\n')
        end++;
    if (my_malloc_c(&line, end - start + 1) == KO)
        return NULL;
    strncpy(line, start, end - start);
    line[end - start] = '\0';
    return line;
}

static char *execute_str(char **lines, char *str)
{
    char *line = NULL;
    int size = 0;
    int len = 0;

    if (!lines || !str)
        return NULL;
    for (size = 0; lines[size]; size++);
    for (len = 0; str[len]; len++);
    for (int i = size - 1; i >= 0; i--) {
        if (my_strncmp(lines[i], str, len) == 0) {
            line = my_strdup(lines[i]);
            free_array(lines);
            return line;
        }
    }
    free_array(lines);
    return NULL;
}

static int execution(main_data_t *data, array_t *input, int start, char **line)
{
    char *cmd = NULL;
    char *f = NULL;

    if (!data || !input || !line)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    cmd = input->data[start];
    f = read_file(data);
    if (!f)
        return KO;
    if (cmd[1] == '!' && my_strlen(cmd) < 3)
        *line = execute_last(f);
    else if (my_str_isnum(&cmd[1]))
        *line = execute_n(f, my_atoi(((char *)input->data[start]) + 1));
    else
        *line = execute_str(str_to_str_array(f, "\n", false),
        ((char *)input->data[start]) + 1);
    return OK;
}

static int exe_line(main_data_t *data, char *line)
{
    array_t *inputs_save = NULL;
    char *input_save = NULL;

    if (!data || !line)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    input_save = data->input;
    inputs_save = data->inputs;
    data->input = line;
    if (do_input(data) == KO)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    data->input = input_save;
    data->inputs = inputs_save;
    return OK;
}

int builtin_exclamation(main_data_t *data, array_t *input, int start)
{
    char *line = NULL;

    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (data->excla_depth) {
        data->excla_depth = false;
        return err_system(data, OK, input->data[start],
        "Stopped, can't call another '!' within a '!'");
    }
    data->excla_depth = true;
    if (execution(data, input, start, &line) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (!data->input)
        return err_system(data, OK, input->data[start],
        "Can't find a corresponding command in history");
    if (exe_line(data, line) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    data->excla_depth = false;
    return OK;
}
