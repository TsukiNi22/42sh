/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** autocompletion.c
*/

#include "minishell.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

void enable_raw_mode(struct termios *original)
{
    struct termios raw;

    tcgetattr(STDIN_FILENO, original);
    raw = *original;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    printf(">> ");
    fflush(stdout);
}

static void suggest(char **prefix, int pos)
{
    int found = 0;
    size_t len = strlen(*prefix);

    prefix[0][pos] = '\0';
    printf("\n");
    for (int i = 0; builtin_array_name[i] != NULL; i++)
        if (strncmp(builtin_array_name[i], *prefix, len) == 0) {
            printf("%s  ", builtin_array_name[i]);
            found = 1;
        }
    if (!found)
        printf("No command matching : %s", *prefix);
    printf("\n>> %s", *prefix);
    fflush(stdout);
}

static int input_cmd(char **str, int pos, char **input)
{
    str[0][pos] = '\0';
    *input = strdup(str[0]);
    free(*str);
    return pos;
}

static void backspace(char **str, int *pos)
{
    if (*pos > 0) {
        *pos -= 1;
        str[0][*pos] = '\0';
        printf("\b \b");
        fflush(stdout);
    }
}

int input_handler(char **input)
{
    char *str = malloc(sizeof(char) * MAX_INPUT_STR);
    int pos = 0;
    char c = 0;

    str[pos] = '\0';
    while (1) {
        c = getchar();
        if (c == '\n')
            return input_cmd(&str, pos, input);
        if (c == 127 || c == '\b')
            backspace(&str, &pos);
        if (c == '\t')
            suggest(&str, pos);
        if (c != '\t' && c != '\b' && isprint(c) && pos < MAX_INPUT_STR - 1) {
            str[pos] = c;
            pos++;
            putchar(c);
            fflush(stdout);
        }
    }
}
