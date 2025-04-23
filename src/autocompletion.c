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
#include <errno.h>

static size_t get_array_len(const char **arr)
{
    size_t len = 0;

    for (; arr[len]; len++);
    return len;
}

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
    size_t i = get_array_len(builtin_array_name);
    char **cmds = malloc(sizeof(char *) * i);
    char *path = strdup(ht_search(env, "PATH"));
    char *ptr = strtok(path, ":");

    if (!path || !cmds)
        return NULL;
    for (size_t j = 0; j < i; j++)
        cmds[j] = strdup(builtin_array_name[j]);
    while (ptr) {
        get_from_path(&cmds, ptr, &i);
        ptr = strtok(NULL, ":");
        if (!cmds)
            return NULL;
    }
    return cmds;
}

static void print_prompt(main_data_t *data, char *str)
{
    printf("\n");
    set_prompt(data);
    if (str)
        printf("%s", str);
    fflush(stdout);
}

static size_t nb_files(void)
{
    DIR *d = opendir(".");
    struct dirent *dp = readdir(d);
    size_t len = 0;

    for (; dp; len++)
        dp = readdir(d);
    return (len + 1);
}

static char **list_files(void)
{
    DIR *d = opendir(".");
    struct dirent *dp = readdir(d);
    size_t len = nb_files();
    char **cmds = malloc(sizeof(char *) * (len + 1));

    for (size_t i = 0; dp; i++) {
        cmds[i] = strdup(dp->d_name);
        dp = readdir(d);
    }
    cmds[len] = NULL;
    return cmds;
}

static size_t get_index(char *str)
{
    int i = strlen(str);

    for (; str[i] != ' ' && i > 0; i--);
    for (; str[i] == ' '; i++);
    if (i < 0)
        i = 0;
    return i;
}

static size_t get_space(char *str)
{
    size_t i = 0;
    size_t space = 0;

    for (; str[i] != ' ' && str[i] != '\0'; i++);
    if (i == 0)
        return space;
    for (; str[i] == ' '; i++)
        space++;
    return space;
}

static void suggest(char **prefix, int pos, main_data_t *data)
{
    int found = 0;
    size_t len = 0;
    char **cmds = NULL;
    size_t index = 0;
    size_t space = get_space(prefix[0]);

    if (!space)
        cmds = get_cmds(data->env);
    else {
        cmds = list_files();
        index = get_index(prefix[0]);
    }
    len = strlen(&prefix[0][index]);
    prefix[0][pos] = '\0';
    printf("\n");
    if (!cmds)
        return;
    for (size_t i = 0; cmds[i]; i++) {
        if (strncmp(cmds[i], &prefix[0][index], len) == 0) {
            printf("%s  ", cmds[i]);
            found = 1;
        }
    }
    if (cmds)
        free(cmds);
    fflush(stdout);
    if (!found) {
        if (!space)
            printf("No command matching : %s", &prefix[0][index]);
        else
            printf("No file or directory matching : %s", &prefix[0][index]);
    }
    print_prompt(data, prefix[0]);
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

int input_handler(main_data_t *data)
{
    char *str = malloc(sizeof(char) * MAX_INPUT_STR);
    int pos = 0;
    char c = 0;
    ssize_t read_bytes = 0;

    str[pos] = '\0';
    while (1) {
        if (sigint(false, false) == 1) {
            sigint(true, false);
            pos = 0;
            str[pos] = '\0';
            print_prompt(data, NULL);
            continue;
        }
        read_bytes = read(STDIN_FILENO, &c, 1);
        if (read_bytes < 0 && errno == EINTR) {
            continue;
        }
        if (c == VEOF) {
            printf("exit\n");
            free(str);
            data->out = true;
            break;
        }
        if (c == '\n')
            return input_cmd(&str, pos, &(data->input));
        if (c == 127 || c == '\b')
            backspace(&str, &pos);
        if (c == '\t')
            suggest(&str, pos, data);
        if (c != '\t' && c != '\b' && isprint(c) && pos < MAX_INPUT_STR - 1) {
            str[pos] = c;
            pos++;
            putchar(c);
            fflush(stdout);
        }
    }
    return EXIT_SUCCESS;
}
