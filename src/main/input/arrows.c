/*
** EPITECH PROJECT, 2025
** arrows
** File description:
** movement of arrows
*/

#include "memory.h"
#include "minishell.h"
#include "error.h"
#include "my_string.h"
#include "file.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

static int directional_arrow(char *seq, int *cursor_pos, int str_len)
{
    if (seq[1] == 'D' && *cursor_pos > 0) {
        (*cursor_pos)--;
        write(STDOUT_FILENO, "\033[D", 3);
        return 1;
    }
    if (seq[1] == 'C' && *cursor_pos < str_len) {
        (*cursor_pos)++;
        write(STDOUT_FILENO, "\033[C", 3);
        return 1;
    }
    return 0;
}

static char *print_input(main_data_t *data, char *str, int *cursor_pos)
{
    char *var = NULL;
    int len = 0;

    write(STDOUT_FILENO, "\33[2K\r", 5);
    print_prompt(data, "no_\n");
    len = my_strlen(str);
    if (len > MAX_INPUT_STR - 1)
        len = MAX_INPUT_STR - 1;
    my_malloc_c(&var, MAX_INPUT_STR + 1);
    if (str) {
        write(STDOUT_FILENO, str, len);
        for (int i = 0; i < MAX_INPUT_STR - 1 && str[i]; i++)
            var[i] = str[i];
    }
    *cursor_pos = my_strlen(var);
    if (*cursor_pos == KO)
        *cursor_pos = 0;
    return var;
}

static char *last_args(main_data_t *data, int len_array, char **array,
    int *cursor_pos)
{
    data->nb_press = 0;
    if (len_array == 0) {
        write(STDOUT_FILENO, "\33[2K\r", 5);
        print_prompt(data, "no_\n");
        return NULL;
    }
    return print_input(data, array[0], cursor_pos);
}

static char *up_arrow(main_data_t *data, int len_array, int *cursor_pos,
    bool down)
{
    char *path = NULL;
    char *file = NULL;
    char **array = NULL;

    path = get_full_path(ht_search(data->env, "HOME"), HISTORY_FILE);
    file = get_file(path);
    array = str_to_str_array(file, "\n", false);
    if (!file || !path || !array)
        return NULL;
    for (; array[len_array]; len_array++);
    if (data->nb_press > 0 && down == true)
        data->nb_press--;
    else if (down == false)
        data->nb_press++;
    if (data->nb_press >= len_array)
        return last_args(data, len_array, array, cursor_pos);
    return print_input(data, array[len_array - data->nb_press], cursor_pos);
}

int arrows(main_data_t *data, int *cursor_pos, int str_len, char **str)
{
    char seq[2];
    int len_array = 0;

    if (read(STDIN_FILENO, &seq[0], 1) <= 0)
        return 0;
    if (read(STDIN_FILENO, &seq[1], 1) <= 0)
        return 0;
    if (seq[0] == '[') {
        if (seq[1] == 'A' || seq[1] == 'B') {
            free(*str);
            *str = up_arrow(data, len_array, cursor_pos, (seq[1] == 'B'));
            return 1;
        }
        if (directional_arrow(seq, cursor_pos, str_len) == 1)
            return 1;
    }
    return 0;
}
