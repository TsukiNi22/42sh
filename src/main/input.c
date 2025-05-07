/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** input.c
*/

#include "error.h"
#include "minishell.h"
#include "my_string.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

static int input_cmd(char **str, int pos, char **input)
{
    if (!str)
        return EXIT_FAILURE;
    str[0][pos] = '\0';
    *input = strdup(str[0]);
    if (str[0])
        free(str[0]);
    return EXIT_FAILURE;
}

static void backspace(char **str, int *pos)
{
    if (!str)
        return;
    if (*pos > 0) {
        *pos -= 1;
        str[0][*pos] = '\0';
        printf("\b \b");
        fflush(stdout);
    }
}

static size_t cc_handler(main_data_t *data, int *pos, char **str)
{
    if (!str)
        return EXIT_FAILURE;
    sigint(true, false);
    *pos = 0;
    str[0][*pos] = '\0';
    print_prompt(data, NULL);
    return EXIT_SUCCESS;
}

static void add_char(char c, char **str, int *pos)
{
    if (!str)
        return;
    str[0][*pos] = c;
    *pos += 1;
    putchar(c);
    fflush(stdout);
}

static size_t characters_handling(main_data_t *data, char **str, int *pos,
    char c)
{
    if (!str)
        return EXIT_FAILURE;
    if (c == '\n')
        return input_cmd(str, *pos, &(data->input));
    if (c == 127 || c == '\b')
        backspace(str, pos);
    if (c == '\t')
        suggest(str, *pos, data);
    if (c == 27) {
        arrows(data, pos, my_strlen(*str), str);
        return EXIT_SUCCESS;
    }
    if (c != '\t' && c != '\b' && isprint(c) && *pos < MAX_INPUT_STR - 1)
        add_char(c, str, pos);
    return EXIT_SUCCESS;
}

static void eof_handling(main_data_t *data, char *str)
{
    printf("exit");
    free(str);
    data->input = strdup("\0");
    data->out = true;
}

int input_handler(main_data_t *data)
{
    char *str = malloc(sizeof(char) * MAX_INPUT_STR);
    int pos = 0;
    char c = 0;

    str[pos] = '\0';
    while (1) {
        if (data->pty)
            return pty_input_handler(data);
        if (sigint(false, false) == 1 && cc_handler(data, &pos, &str) == 0)
            continue;
        if (read(STDIN_FILENO, &c, 1) < 0 && errno == EINTR)
            continue;
        if (c == VEOF) {
            eof_handling(data, str);
            break;
        }
        if (characters_handling(data, &str, &pos, c) != EXIT_SUCCESS)
            return ERR;
    }
    return EXIT_SUCCESS;
}
