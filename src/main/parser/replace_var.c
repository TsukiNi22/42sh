/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** replace_var.c
*/

#include "my_string.h"
#include "minishell.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int replace_var_input(main_data_t *data, char **input, char *var)
{
    char *value = NULL;

    if (!input || !var || !data->env)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (!value)   
        return err_system(data, OK, var, "Undefined variable");
    for (int i = 0; i < (var - 1) - *input; i++) {
        return OK;
    }
    return OK;
}

static int check_env(main_data_t *data, char *var)
{
    char c = '\0';
    int nb_lettre_var;

    if (!data)
        return OK;
    c = *var;
    if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) && c != '_')
        return OK;
    for (nb_lettre_var = 0; var; nb_lettre_var++) {
        if (!((var[nb_lettre_var] >= '0' && var[nb_lettre_var] <= '9')
            || var[nb_lettre_var] == '_'
            || (var[nb_lettre_var] >= 'A' && var[nb_lettre_var] <= 'Z') 
            || (var[nb_lettre_var] >= 'a' && var[nb_lettre_var] <= 'z')))
            break;
    }
    *(var - 1) = '\0';
    var[nb_lettre_var] = '\0';
    replace_var_input(data, &data->input, var);
    return OK;
}

int replace_var(main_data_t *data)
{
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (int i = 0; data->input[i]; i++) {
        if ((i == 0 || data->input[i - 1] != data->esc_char)
            && data->input[i] == '$')
            check_env(data, &data->input[i + 1]);
    }
    return OK;
}
