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

static int len_of_array(char **array)
{
    int len = 0;

    for (; array[len]; len++);
    return len;
}

static char *print_input(main_data_t *data, char *str)
{
    write(STDOUT_FILENO, "\33[2K\r", 5);
    print_prompt(data, "no_\n");
    if (str)
        write(STDOUT_FILENO, str, my_strlen(str));
    if (!str)
        my_malloc_c(&str, MAX_INPUT_STR + 1);
    return str;
}

static char *last_args(main_data_t *data, int len_array, char **array,
    int *cursor_pos)
{
    char *str = NULL;

    data->nb_press = 0;
    *cursor_pos = my_strlen(array[len_array - (data->nb_press + 1)]);
    my_malloc_c(&str, MAX_INPUT_STR + 1);
    for (int i = 0; i < array[len_array - (data->nb_press + 1)][i]; i++)
        str[i] = array[len_array - (data->nb_press + 1)][i];
    return str;
}

static char *up_arrow(main_data_t *data, int len_array, int *cursor_pos,
    bool down)
{
    char *path = NULL;
    char *file = NULL;
    char **array = NULL;
    int len = 0;

    path = get_full_path(ht_search(data->env, "HOME"), HISTORY_FILE);
    file = get_file(path);
    if (!file || !path)
        return malloc(sizeof(char) * (MAX_INPUT_STR + 1));
    array = str_to_str_array(file, "\n", false);
    len_array = len_of_array(array);
    if (data->nb_press > 0 && down == true)
        data->nb_press--;
    else if (down == false)
        data->nb_press++;
    if (data->nb_press >= len_array)
        return last_args(data, len_array, array, cursor_pos);
    len = my_strlen(array[len_array - data->nb_press]);
    *cursor_pos = len * (len != KO);
    return print_input(data, array[len_array - data->nb_press]);
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
