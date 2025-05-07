/*
** EPITECH PROJECT, 2025
** main.c
** File description:
** Main of the project
*/

#include "write.h"
#include "my_string.h"
#include "memory.h"
#include "define.h"
#include "hashtable.h"
#include "minishell.h"
#include "error.h"
#include <stdlib.h>
#include <stdbool.h>

static int is_a_pty(main_data_t *data)
{
    char const *term = getenv("TERM");

    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (!data->pty && !term)
        data->pty = true;
    if (!data->pty)
        return OK;
    if (ht_insert(data->env, my_strdup("TERM"),
        my_strdup("minimal"), &free_hash_data_str) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (ht_insert(data->env, my_strdup("HISTSIZE"),
        my_itoa(HISTORY_SIZE), &free_hash_data_str) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}

static int set_env(main_data_t *data, char const *env[])
{
    char *name = NULL;
    char *val = NULL;

    if (!data || !env)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    data->env = new_hashtable(&hash, DEFAULT_HASH_SIZE);
    if (!data->env)
        return my_putstr(STDIN, "mysh: env is NULL\n");
    for (int i = 0; env[i]; i++) {
        name = my_strdup(env[i]);
        if (!name)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        val = name;
        for (; val && val[0] != '='; val++);
        name[val - name] = '\0';
        val = my_strdup(&val[1]);
        if (ht_insert(data->env, name, val, &free_hash_data_str) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    return is_a_pty(data);
}

int main(int const argc, char const *argv[], char const *env[])
{
    main_data_t data = {0};
    int res = 0;

    if (!argv)
        return err_prog(PTR_ERR, EPITECH_ERR, ERR_INFO);
    if (argc > 2 || (argc == 2 && my_strcmp(argv[1], "-t") != 0))
        return err_custom("Usage: 42sh [-t]", EPITECH_ERR, ERR_INFO);
    data.pty = (argc == 2);
    if (set_env(&data, env) == KO)
        return err_prog(UNDEF_ERR, EPITECH_ERR, ERR_INFO);
    res = minishell(&data);
    if (res == KO || res == FATAL_ERR) {
        if (res != FATAL_ERR)
            free_data(&data);
        my_putstr(STDERR, "mysh: An error has occurred, "
        "execution has been interrupted.\n");
        return EPITECH_ERR;
    }
    free_data(&data);
    return data.return_value;
}
