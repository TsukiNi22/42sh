/*
** EPITECH PROJECT, 2025
** check_accesible_dir.c
** File description:
** You know, I don t think there are good or bad descriptions,
** for me, life is above all about functions...
*/

#include "error.h"
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>

bool is_accesible_dir(char const *path)
{
    struct stat st = {0};

    if (stat(path, &st) == KO)
        return err_prog(UNDEF_ERR, false, ERR_INFO);
    if (!S_ISDIR(st.st_mode))
        return false;
    if (access(path, R_OK) == KO)
        return false;
    return true;
}
