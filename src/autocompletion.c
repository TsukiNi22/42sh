/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** autocompletion.c
*/

#include "minishell.h"
#include <ctype.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static void get_from_path(char ***cmds, char *path, size_t *i)
{
    DIR *d = opendir(path);
    struct dirent *dp;

    if (!d) {
        closedir(d);
        return;
    }
    dp = readdir(d);
    while (dp) {
        if (dp->d_type == DT_REG) {
            cmds[0] = realloc(cmds[0], sizeof(char *) * (*i + 2));
            cmds[0][*i] = strdup(dp->d_name);
            *i += 1;
            cmds[0][*i] = NULL;
        }
        dp = readdir(d);
    }
    closedir(d);
}

static char **get_cmds(hashtable_t *env)
{
    char **cmds = malloc(sizeof(char *));
    char *path = strdup(ht_search(env, "PATH"));
    char *ptr = strtok(path, ":");
    size_t i = 0;

    if (!path || !cmds)
        return NULL;
    while (ptr) {
        get_from_path(&cmds, ptr, &i);
        ptr = strtok(NULL, ":");
        if (!cmds)
            return NULL;
    }
    return cmds;
}

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

static void suggest(char **prefix, int pos, hashtable_t *env)
{
    int found = 0;
    size_t len = strlen(prefix[0]);
    char **cmds = get_cmds(env);

    prefix[0][pos] = '\0';
    printf("\n");
    if (!cmds)
        return;
    for (size_t i = 0; cmds[i]; i++) {
        if (strncmp(cmds[i], prefix[0], len) == 0) {
            printf("%s  ", cmds[i]);
            found = 1;
        }
    }
    if (cmds)
        free(cmds);
    if (!found)
        printf("No command matching : %s", prefix[0]);
    printf("\n>> %s", prefix[0]);
    fflush(stdout);
}

static int input_cmd(char **str, int pos, char **input)
{
    str[0][pos] = '\0';
    *input = strdup(str[0]);
    if (str[0])
        free(str[0]);
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

int input_handler(char **input, hashtable_t *env)
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
            suggest(&str, pos, env);
        if (c != '\t' && c != '\b' && isprint(c) && pos < MAX_INPUT_STR - 1) {
            str[pos] = c;
            pos++;
            putchar(c);
            fflush(stdout);
        }
    }
}
