/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** replace_var.c
*/

#include "my_string.h"
#include "memory.h"
#include "minishell.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

static int replace_var_input(main_data_t *data, char *var,
    char *end_input, char c)
{
    char *new_input = NULL;
    char *value = NULL;
    int count = 0;

    value = ht_search(data->env, var);
    if (!value)
        return err_system(data, OK, var, "Undefined variable");
    *end_input = c;
    count = (var - 1) - data->input;
    count += my_strlen(value);
    count += my_strlen(end_input);
    if (my_malloc_c(&new_input, count + 1) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (!my_strcat(new_input, data->input)
        || !my_strcat(new_input, value)
        || !my_strcat(new_input, end_input))
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    free(data->input);
    data->input = new_input;
    return OK;
}

static bool is_var_char(char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        return true;
    return false;
}

static int check_env(main_data_t *data, char *var, bool *done)
{
    char c = '\0';
    int nb_lettre_var = 0;

    if (!data || !var || !done)
        return OK;
    *done = true;
    c = *var;
    if (!is_var_char(c) && c != '_')
        return OK;
    for (nb_lettre_var = 0; var; nb_lettre_var++) {
        if (!((var[nb_lettre_var] >= '0' && var[nb_lettre_var] <= '9')
            || var[nb_lettre_var] == '_'
            || is_var_char(var[nb_lettre_var])))
            break;
    }
    *(var - 1) = '\0';
    c = var[nb_lettre_var];
    var[nb_lettre_var] = '\0';
    replace_var_input(data, var, var + nb_lettre_var, c);
    return OK;
}

static int check_home(main_data_t *data, char *var, bool *done)
{
    char *new_input = NULL;
    char *my_home = NULL;
    int count = 0;

    if (!data || !var || !done)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    *done = true;
    my_home = ht_search(data->env, "HOME");
    if (!my_home)
        return err_system(data, OK, NULL, "HOME is not defined");
    count = (my_strlen(data->input) - 1) + my_strlen(my_home);
    *var = '\0';
    if (my_malloc_c(&new_input, count + 1) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (!my_strcat(new_input, data->input) || !my_strcat(new_input, my_home)
        || !my_strcat(new_input, var + 1))
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    free(data->input);
    data->input = new_input;
    return OK;
}

int replace_var(main_data_t *data, bool *done)
{
    if (!data || !done)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (int i = 0; data->input[i]; i++) {
        if ((i == 0 || data->input[i - 1] != data->esc_char)
            && data->input[i] == '$'
            && check_env(data, &data->input[i + 1], done) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    for (int i = 0; data->input[i]; i++) {
        if ((i == 0 || data->input[i - 1] != data->esc_char)
            && (i == 0 || data->input[i - 1] != '~')
            && data->input[i + 1] != '~'
            && data->input[i] == '~'
            && check_home(data, &data->input[i], done) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    return OK;
}
