/*
** EPITECH PROJECT, 2025
** arrows
** File description:
** movement of arrows
*/

#include "minishell.h"
#include "error.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int arrows(int *cursor_pos, int str_len)
{
    char seq[2];

    if (read(STDIN_FILENO, &seq[0], 1) <= 0)
        return 0;
    if (read(STDIN_FILENO, &seq[1], 1) <= 0)
        return 0;
    if (seq[0] == '[') {
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
