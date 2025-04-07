/*
** EPITECH PROJECT, 2025
** parser.c
** File description:
** Parser the given input
*/

#include "my_string.h"
#include "minishell.h"
#include "error.h"
#include <stdlib.h>

static int set_input(char *conditional_string[], char *redirection_string[],
    char **input, int *i)
{
    int lens[3] = {0};

    if (!conditional_string || !redirection_string || !input || !i)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    lens[0] = my_strlen(conditional_string[0]);
    lens[1] = my_strlen(conditional_string[1]);
    lens[2] = my_strlen(redirection_string[0]);
    if (lens[0] == KO || lens[1] == KO || lens[2] == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (my_strncmp(&(*input)[*i], conditional_string[0], lens[0]) == 0)
        *input = &(*input)[*i + lens[0]];
    else if (my_strncmp(&(*input)[*i], conditional_string[1], lens[1]) == 0)
        *input = &(*input)[*i + lens[1]];
    else
        *input = &(*input)[*i + lens[2]];
    *i = -1;
    return OK;
}

static int set_spe_char(char *conditional_string[],
    array_t *array, char *input, int *val)
{
    int lens[2] = {0};

    if (!conditional_string || !array || !input || !val)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    lens[0] = my_strlen(conditional_string[0]);
    lens[1] = my_strlen(conditional_string[1]);
    if (lens[0] == KO || lens[1] == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (my_strncmp(input, conditional_string[0], lens[0]) == 0)
        *val = AND;
    else if (my_strncmp(input, conditional_string[1], lens[1]) == 0)
        *val = OR;
    else
        *val = PIPE;
    if (add_array(array, val) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}

static int set_spe_string(char *conditional_string[],
    array_t *array, char *input)
{
    int *vals[2] = {NULL};

    if (!conditional_string || !array || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (add_array(array, new_array()) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    vals[0] = malloc(sizeof(int));
    vals[1] = malloc(sizeof(int));
    if (!vals[0] || !vals[1])
        return err_prog(MALLOC_ERR, KO, ERR_INFO);
    *(vals[0]) = -1;
    if (add_array(array->data[array->len -1], vals[0]) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return set_spe_char(conditional_string,
    array->data[array->len - 1], input, vals[1]);
}

static bool is_empty(char *input)
{
    if (!input)
        return err_prog(PTR_ERR, true, ERR_INFO);
    for (int i = 0; input[i]; i++) {
        if (input[i] != ' ' && input[i] != '\t')
            return false;
    }
    return true;
}

int parser(main_data_t *data, array_t *array, char **input, int *i)
{
    char c = '\0';

    if (!data || !array || !input || !i)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    c = (*input)[*i];
    (*input)[*i] = '\0';
    if (!is_empty(*input) && cmd_parser(data, array, *input, 0) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    (*input)[*i] = c;
    if (c && set_spe_string(data->conditional_string,
        array, &(*input)[*i]) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return set_input(data->conditional_string,
    data->redirection_string, input, i);
}
