/*
** EPITECH PROJECT, 2025
** sigtstp.c
** File description:
** Handle sigtstp signal
*/

#include "define.h"
#include "write.h"
#include "macro.h"
#include "error.h"
#include <stdlib.h>

void handle_sigtstp(UNUSED int sig)
{
    if (my_putstr(STDOUT, "\b\b  \b\b") == KO)
        exit(EPITECH_ERR);
}
