/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** autocompletion.c
*/

#include "minishell.h"
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

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

static char **init_cmds(main_data_t *data, char **prefix, size_t *index,
    size_t space)
{
    char **cmds = NULL;

    if (!space)
        cmds = get_cmds(data->env);
    else
        cmds = list_files();
    if (!cmds)
        return NULL;
    *index = get_index(prefix[0]);
    printf("\n");
    return cmds;
}

static size_t print_cmds(char **cmds, char **prefix, size_t index)
{
    size_t len = strlen(&prefix[0][index]);
    size_t found = 0;

    for (size_t i = 0; cmds[i]; i++) {
        if (strncmp(cmds[i], &prefix[0][index], len) == 0) {
            printf("%s  ", cmds[i]);
            found = 1;
        }
    }
    return found;
}

void suggest(char **prefix, int pos, main_data_t *data)
{
    size_t index = 0;
    size_t space = get_space(prefix[0]);
    char **cmds = init_cmds(data, prefix, &index, space);

    prefix[0][pos] = '\0';
    if (!print_cmds(cmds, prefix, index)) {
        if (!space)
            printf("No command matching : %s", &prefix[0][index]);
        else
            printf("No file or directory matching : %s", &prefix[0][index]);
    }
    if (cmds)
        free(cmds);
    fflush(stdout);
    print_prompt(data, prefix[0]);
}
