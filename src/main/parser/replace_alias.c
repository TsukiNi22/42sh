/*
** EPITECH PROJECT, 2025
** replace_alias.c
** File description:
** replace alias in input
*/

#include "memory.h"
#include "hashtable.h"
#include "minishell.h"
#include "error.h"
#include <stdlib.h>

int replace_alias(main_data_t *data, bool *done)
{
    char *str = NULL;
    char *save = NULL;
    int size = KO;

    if (!data || !done)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (int i = 0; data->input[i]; i++) {
        save = NULL;
        size = KO;
        for (int j = 0; data->input[i + j]; j++) {
            str = my_strndup(&data->input[i], j);
            if (ht_search(data->alias, str)) {
                if (save)
                    free(save);
                save = str;
                size = j;
            } else
                free(str);
        }
        if (save) {
            #include <stdio.h>
            printf("Alias: |%s| |%d| |%d| |%s| |%s|\n", save, i, size, data->input, &data->input[i]);
            i += size;
            free(save);
        }
    }
    return OK;
}
