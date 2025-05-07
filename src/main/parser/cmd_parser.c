/*
** EPITECH PROJECT, 2025
** parser.c
** File description:
** Parser the given input
*/

#include "write.h"
#include "define.h"
#include "memory.h"
#include "my_string.h"
#include "minishell.h"
#include "error.h"
#include <stdlib.h>

static int set_redirect_nb(array_t *array)
{
    int *val = NULL;

    if (!array)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    val = malloc(sizeof(int));
    if (!val)
        return err_prog(MALLOC_ERR, KO, ERR_INFO);
    *val = 0;
    if (add_array(array, val) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
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

static int set_cmd(array_t *array, char **ptr, char *input, bool spe)
{
    char *str = NULL;
    int var[2] = {0};

    if (!array || !ptr || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    var[0] = input - *ptr;
    if (var[0] > 0 && my_malloc_c(&str, var[0] + 1) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    for (int i = 0; i < var[0]; i++) {
        spe = (i > 0 && (*ptr)[i - 1] == '\\')
        || ((*ptr)[i] != '\\' && (*ptr)[i] != '\'' && (*ptr)[i] != '\"'
        && (*ptr)[i] != '(' && (*ptr)[i] != ')');
        str[i - var[1]] = (*ptr)[i] * spe;
        var[1] += !spe;
    }
    if (var[0] > 0 && add_array(array, str) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    *ptr = input + 1;
    return OK;
}

static bool is_redirection_string(char *input,
    char *redirection_string[], int *lens)
{
    if (!input || !redirection_string || !lens)
        return err_prog(PTR_ERR, false, ERR_INFO);
    for (int i = 0; i < 4; i++) {
        if (my_strncmp(input, redirection_string[i], lens[i]) == 0)
            return true;
    }
    return false;
}

static int clear_n_char(char *input, int n)
{
    int i = 0;

    if (!input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (i = n; input[i]; i++)
        input[i - n] = input[i];
    input[i - n] = '\0';
    return OK;
}

static int set_spe_char(char *redirection_string[],
    array_t *array, char *input, int *lens)
{
    int *val = NULL;
    int i = 0;

    if (!redirection_string || !array || !input || !lens)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    val = malloc(sizeof(int));
    if (!val)
        return err_prog(MALLOC_ERR, KO, ERR_INFO);
    for (i = 0; i < 4; i++) {
        if (my_strncmp(input, redirection_string[i], lens[i]) == 0)
            break;
    }
    *val = D_RIGHT + i;
    if (add_array(array, val) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return clear_n_char(input, lens[i]);
}

static int set_redirection_arg(array_t *array, char *input)
{
    char *str = NULL;
    char *ptr = NULL;
    char c = '\0';
    int i = 0;

    if (!array || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (i = 0; input[i] && (input[i] == ' ' || input[i] == '\t'); i++);
    ptr = &input[i];
    for (; input[i] && input[i] != ' ' && input[i] != '\t'; i++);
    c = input[i];
    input[i] = '\0';
    str = my_strdup(ptr);
    input[i] = c;
    if (!str || add_array(array, str) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return clear_n_char(input, i);
}

static int extract_redirection(main_data_t *data, array_t *array, char *input)
{
    int lens[4] = {0};

    if (!data || !array || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (int i = 1; i < 5; i++) {
        lens[i - 1] = my_strlen(data->redirection_string[i]);
        if (lens[i - 1] == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    for (int i = 0; input[i]; i++) {
        if (data->esc_char == input[i - 1 * (i > 0)] || !is_redirection_string(
            &input[i], &(data->redirection_string[1]), lens))
            continue;
        if (set_spe_char(&(data->redirection_string[1]),
            array, &input[i], lens) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        if (set_redirection_arg(array, &input[i]) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    return OK;
}

static int cmd_init(main_data_t *data, array_t *array, char *input)
{
    if (!data || !array || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (add_array(array, new_array()) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (set_redirect_nb(array->data[array->len - 1]) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (extract_redirection(data, array->data[array->len - 1], input) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    *((int *) ((array_t *) array->data[array->len - 1])->data[0]) =
    ((array_t *) array->data[array->len - 1])->len - 1;
    return OK;
}

int cmd_parser(main_data_t *data, array_t *array, char *input, int i)
{
    bool spe[3] = {false};
    char *ptr = input;

    if (!data || !array || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (cmd_init(data, array, input) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    for (i = 0; input[i]; i++) {
        if (is_spe(input, i, spe, data->esc_char))
            continue;
        if ((input[i] == ' ' || input[i] == '\t')
            && set_cmd(array->data[array->len - 1], &ptr, &input[i], 0) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        i += 2 * (input[i + 1] == data->esc_char) * !(!input[i + 1]);
    }
    data->err_sys = (spe[0] || spe[1] || spe[2]);
    if (!data->pty && (spe[0] || spe[1] || spe[2]))
        return my_printf("Unmatched '%c'.\n",
        '\'' * spe[0] + '\"' * spe[1] + '(' * spe[2]);
    return set_cmd(array->data[array->len - 1], &ptr, &input[i], 0);
}
