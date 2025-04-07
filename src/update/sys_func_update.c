/*
** EPITECH PROJECT, 2025
** sys_func_update.c
** File description:
** Update the both sys_func_path && sys_func variable
*/

#include "define.h"
#include "memory.h"
#include "write.h"
#include "my_string.h"
#include "hashtable.h"
#include "minishell.h"
#include "error.h"
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdbool.h>

static void free_path(char ***array)
{
    if (!array || !(*array))
        return;
    for (int i = 0; (*array)[i]; i++)
        free((*array)[i]);
    free(*array);
    *array = NULL;
}

static int check_path(main_data_t *data)
{
    int res = OK;

    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (!data->path_changed)
        return OK;
    data->path_changed = false;
    for (int i = 0; data->env_path[i]; i++) {
        if (!is_accesible_dir(data->env_path[i]))
            res = my_printf("%OThe path '%s'"
            " in the environement variable 'PATH'"
            ", does not exist or insufficient permission to read. "
            "Can't obtain system function.\n", STDERR, data->env_path[i]);
        if (res != OK)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    return OK;
}

static bool is_dir(char const *path, char const *name)
{
    char *full_path = NULL;
    struct stat st = {0};

    if (!path || !name)
        return err_prog(PTR_ERR, false, ERR_INFO);
    full_path = get_full_path(path, name);
    if (!full_path)
        return err_prog(UNDEF_ERR, false, ERR_INFO);
    if (stat(full_path, &st) == KO)
        return err_custom(strerror(errno), false, ERR_INFO);
    if (!S_ISDIR(st.st_mode))
        return false;
    free(full_path);
    return true;
}

static int set_sys_func(main_data_t *data, char const *path)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;

    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    dir = opendir(path);
    if (!dir)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    for (entry = readdir(dir); entry; entry = readdir(dir)) {
        if (entry->d_name[0] == '.' || is_dir(path, entry->d_name))
            continue;
        if (ht_insert(data->sys_func, my_strdup(entry->d_name),
            my_strdup(entry->d_name), &free_hash_data_str) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    closedir(dir);
    return OK;
}

static int get_sys_func(main_data_t *data)
{
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    data->sys_func = new_hashtable(&hash, DEFAULT_HASH_SIZE);
    if (!data->sys_func)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    for (int i = 0; data->env_path[i]; i++) {
        if (is_accesible_dir(data->env_path[i])
            && set_sys_func(data, data->env_path[i]) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    return OK;
}

static int malloc_sys_mtime(main_data_t *data)
{
    int nb_path = 0;

    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (nb_path = 0; data->env_path[nb_path]; nb_path++);
    if (!data->sys_mtime || data->nb_mtime != nb_path) {
        if (data->sys_mtime)
            free(data->sys_mtime);
        data->sys_mtime = malloc(sizeof(time_t) * nb_path);
        if (!data->sys_mtime)
            return err_prog(MALLOC_ERR, KO, ERR_INFO);
        for (int i = 0; i < nb_path; i++)
            data->sys_mtime[i] = 0;
        data->nb_mtime = nb_path;
    }
    return OK;
}

static bool file_changed(main_data_t *data)
{
    struct stat st = {0};
    bool changed = false;

    if (!data)
        return err_prog(PTR_ERR, true, ERR_INFO);
    if (malloc_sys_mtime(data) == KO)
        return err_prog(UNDEF_ERR, true, ERR_INFO);
    for (int i = 0; data->env_path[i]; i++) {
        if (!is_accesible_dir(data->env_path[i]))
            continue;
        if (stat(data->env_path[i], &st) == KO)
            return err_prog(UNDEF_ERR, true, ERR_INFO);
        if (data->sys_mtime[i] != st.st_mtime) {
            data->sys_mtime[i] = st.st_mtime;
            changed = true;
        }
    }
    return changed;
}

int sys_func_update(main_data_t *data)
{
    char *path = NULL;

    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    path = ht_search(data->env, "PATH");
    if (!path || my_strcmp(path, "") == 0) {
        if (data->path_changed)
            my_putstr(STDERR, "No PATH found, automaticly use '/bin.'\n");
        path = my_strdup("/bin");
    }
    free_path(&(data->env_path));
    data->env_path = str_to_str_array(path, ":", false);
    if (!data->env_path || check_path(data) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (!file_changed(data))
        return OK;
    if (data->sys_func && delete_hashtable(data->sys_func,
        &free_hash_data_str) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return get_sys_func(data);
}
