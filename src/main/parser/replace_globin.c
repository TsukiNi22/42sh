/*
** EPITECH PROJECT, 2025
** replace_goblin.c
** File description:
** replace goblin in input
*/

#include "memory.h"
#include "my_string.h"
#include "minishell.h"
#include "error.h"
#include <glob.h>
#include <stdlib.h>
#include <stdbool.h>

static int get_gl_size(glob_t *gl)
{
    int size = 0;
    int res = OK;

    if (!gl)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (size_t i = 0; i < gl->gl_pathc; i++) {
        res = my_strlen(gl->gl_pathv[i]);
        if (res == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        size += res + 1 - (i >= gl->gl_pathc - 1);
    }
    return size;
}

static int replace_gl(main_data_t *data, char *ptrs[2], glob_t *gl)
{
    char *str = NULL;
    int size = 0;
    int res = OK;

    if (!data || !ptrs || !gl)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (size = 0; ptrs[1][size]; size++);
    size += ptrs[1] - data->input;
    size += get_gl_size(gl);
    *(ptrs[0]) = '\0';
    if (my_malloc_c(&str, size + 1) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    res += KO * !my_strcat(str, data->input);
    for (size_t i = 0; res == OK && i < gl->gl_pathc; i++)
        res += KO * (!my_strcat(str, gl->gl_pathv[i])
        || (i < gl->gl_pathc - 1 && !my_strcat(str, " ")));
    res += KO * !my_strcat(str, ptrs[1]);
    free(data->input);
    data->input = str;
    return KO * (res != OK);
}

static int set_goblin(main_data_t *data, char *ptrs[2], int *i, bool *done)
{
    char c = '\0';
    glob_t gl = {0};

    if (!data || !ptrs || !i || !done)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    c = *(ptrs[1]);
    *(ptrs[1]) = '\0';
    glob(ptrs[0], 0, NULL, &gl);
    *(ptrs[1]) = c;
    if (gl.gl_pathc > 0)
        *done = true;
    if (gl.gl_pathc > 0 && replace_gl(data, ptrs, &gl) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    *i += get_gl_size(&gl);
    globfree(&gl);
    return OK;
}

static bool is_spe(char *input, int i, bool *spe, char esc_char)
{
    if (!input || !spe)
        return err_prog(PTR_ERR, false, ERR_INFO);
    if (i > 0 && input[i - 1] == esc_char)
        return false;
    if (input[i] == '\'' && !spe[1] && !spe[2])
        spe[0] = !spe[0];
    if (input[i] == '\"' && !spe[0] && !spe[2])
        spe[1] = !spe[1];
    if (input[i] == '(' && !spe[0] && !spe[1] && !spe[2])
        spe[2] = true;
    if (input[i] == ')' && !spe[0] && !spe[1] && spe[2])
        spe[2] = false;
    return (spe[0] || spe[1] || spe[2]);
}

static bool not_spe(char c)
{
    if (c == '\'' || c == '\"' || c == '(')
        return false;
    return true;
}

int replace_goblin(main_data_t *data, bool *done)
{
    char *start = NULL;
    char *end = NULL;
    bool spe[3] = {false};

    if (!data || !done)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (int i = 0; data->input[i]; i++) {
        if (is_spe(data->input, i, spe, data->esc_char))
            continue;
        for (start = &data->input[i]; not_spe(*start)
            && (*start == ' ' || *start == '\t'); start++);
        for (end = start; not_spe(*end)
            && *end != ' ' && *end != '\t' && *end; end++);
        if (not_spe(data->input[i]) && end && not_spe(*start)
            && set_goblin(data, (char *[2]){start, end}, &i, done) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    return OK;
}
