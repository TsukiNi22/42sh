/*
** EPITECH PROJECT, 2025
** sigint.c
** File description:
** Handle sigint signal
*/

#include "macro.h"
#include <stdbool.h>
#include <stdio.h>

size_t sigint(size_t write, size_t value)
{
    static size_t n = 0;

    if (write)
        n = value;
    return n;
}

void handle_sigint(UNUSED int sig)
{
    sigint(true, true);
}
