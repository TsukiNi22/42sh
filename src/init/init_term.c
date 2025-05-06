/*
** EPITECH PROJECT, 2025
** init_term.c
** File description:
** Init term var
*/

#include <SFML/Graphics.h>
#include "minishell.h"
#include "error.h"
#include <stdlib.h>

static int setup_var(terminal_buffer_t *terminal)
{
    if (!terminal)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    terminal->last_apartenance = -1;
    terminal->line_count = 0;
    terminal->actual_ligne = 0;
    return OK;
}

int init_term(main_data_t *data)
{
    sfVideoMode mode = {800, 600, 32};

    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    data->terminal = malloc(sizeof(terminal_buffer_t));
    if (!data->terminal)
        return err_prog(MALLOC_ERR, KO, ERR_INFO);
    data->terminal->window = sfRenderWindow_create(mode,
    "Banana and Shell Handler", sfClose, NULL);
    data->terminal->cursor_clock = sfClock_create();
    if (!data->terminal->window || !data->terminal->cursor_clock)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    data->terminal->font = sfFont_createFromFile(FONT);
    if (!data->terminal->font)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return setup_var(data->terminal);
}
