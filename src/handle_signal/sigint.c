/*
** EPITECH PROJECT, 2025
** sigint.c
** File description:
** Handle sigint signal
*/

#include "define.h"
#include "write.h"
#include "macro.h"
#include "error.h"
#include <stdlib.h>

void handle_sigint(UNUSED int sig)
{
    if (my_putstr(STDOUT, "\b\b  \b\b") == KO)
        exit(EPITECH_ERR);
}
