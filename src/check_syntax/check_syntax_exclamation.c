/*
** EPITECH PROJECT, 2025
** check_syntax_exclamation.c
** File description:
** checks syntax for !
*/

#include "my_string.h"
#include "array.h"
#include "my_string.h"
#include "minishell.h"
#include "file.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>

static int check_exclamation(main_data_t *data)
{
    char *path = NULL;
    char *file = NULL;

    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    path = get_full_path(ht_search(data->env, "HOME"), HISTORY_FILE);
    if (!path)
        return err_system(data, OK, "!", "Can't found the HOME "
        "environement variable");
    file = get_file(path);
    if (!file)
        return err_system(data, OK, "!",
    "Can't get the content of ~/.banana_history");
    free(file);
    return OK;
}

int check_syntax_exclamation(main_data_t *data, array_t *input, int start)
{
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (input->len - start > 1)
        return err_system(data, OK, "!", "Too many arguments");
    if (input->len - start == 1)
        return check_exclamation(data);
    return OK;
}
