/*
** EPITECH PROJECT, 2025
** arrows
** File description:
** movement of arrows
*/

#include "minishell.h"
#include "error.h"
#include "my_string.h"
#include "file.h"
#include <stdlib.h>
#include <unistd.h>

static int len_of_array(char **array)
{
    int len = 0;

    for (; array[len]; len++);
    return len;
}

static char *up_arrow(main_data_t *data, int len_array, int *cursor_pos)
{
    char *path = NULL;
    char *file = NULL;
    char **array = NULL;
    int len = 0;

    path = get_full_path(ht_search(data->env, "HOME"), HISTORY_FILE);
    file = get_file(path);
    if (!file || !path)
        return NULL;
    array = str_to_str_array(file, "\n", false);
    data->nb_press++;
    len_array = len_of_array(array);
    len = my_strlen(array[data->nb_press]);
    *cursor_pos = len;
    if (len_array - data->nb_press < 0)
        return array[len_array - (data->nb_press + 1)];
    write(STDOUT_FILENO, "\33[2K\r", 5);
    print_prompt(data, "no_\n");
    write(STDOUT_FILENO, array[len_array - data->nb_press], len);
    return array[len_array - data->nb_press];
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
        if (seq[1] == 'A') {
            *str = up_arrow(data, len_array, cursor_pos);
            return 1;
        }
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
    }
    return 0;
}
