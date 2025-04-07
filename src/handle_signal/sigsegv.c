/*
** EPITECH PROJECT, 2025
** sigsegv.c
** File description:
** Handle sigsegv signal
*/

#include "define.h"
#include "write.h"
#include "macro.h"
#include "error.h"
#include <stdlib.h>

void handle_sigsegv(UNUSED int sig)
{
    my_putstr(STDERR, "mysh: Got killed will doing somthing.\n");
    exit(EPITECH_ERR);
}
