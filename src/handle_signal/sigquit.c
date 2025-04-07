/*
** EPITECH PROJECT, 2025
** sigquit.c
** File description:
** Handle sigquit signal
*/

#include "define.h"
#include "write.h"
#include "macro.h"
#include "error.h"
#include <stdlib.h>

void handle_sigquit(UNUSED int sig)
{
    if (my_putstr(STDOUT, "\b\b  \b\b") == KO)
        exit(EPITECH_ERR);
}
