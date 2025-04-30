/*
** EPITECH PROJECT, 2025
** display_pty.c
** File description:
** Display the terminal
*/

#include "minishell.h"
#include "error.h"
#include <SFML/Graphics.h>

int get_size(char *line)
{
    int size = 0;

    for (int i = 0; line[i]; i++) {
        if (line[i] == '\t')
            size += 8 * 4;
        else
            size += 8;
    }
    return size;
}

static int prompt(sfRenderWindow *window, sfFont *font, int i)
{
    sfText *text = NULL;

    if (!window || !font)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    text = sfText_create();
    sfText_setString(text, "$> ");
    sfText_setFont(text, font);
    sfText_setCharacterSize(text, 13);
    sfText_setPosition(text, (sfVector2f){10, 10 + i * 18});
    sfText_setColor(text, sfWhite);
    sfRenderWindow_drawText(window, text, NULL);
    sfText_destroy(text);
    return OK;
}

static int text(terminal_buffer_t *terminal, char *str, int x, int y)
{
    sfText *text = NULL;

    if (!terminal || !str)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    text = sfText_create();
    if (!text)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    sfText_setString(text, str);
    sfText_setFont(text, terminal->font);
    sfText_setCharacterSize(text, 13);
    sfText_setPosition(text, (sfVector2f){x, y});
    sfText_setColor(text, sfWhite);
    sfRenderWindow_drawText(terminal->window, text, NULL);
    sfText_destroy(text);
    return OK;
}

static int diplay_past_line(terminal_buffer_t *terminal)
{
    int apartenance = 0;
    bool new = false;

    if (!terminal)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    for (int i = 0; i < terminal->actual_ligne; i++) {
        if ((i == 0 || apartenance != terminal->apartenance[i])
            && terminal->apartenance[i] != -1) {
            prompt(terminal->window, terminal->font, i);
            apartenance = terminal->apartenance[i];
            new = true;
        }
        text(terminal, terminal->lines[i], 10 + 8 * 3 * new, 10 + i * 18);
        new = false;
    }
    return OK;
}

static int diplay_actual_line(terminal_buffer_t *terminal)
{
    if (!terminal)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    prompt(terminal->window, terminal->font, terminal->actual_ligne);
    for (int i = terminal->actual_ligne; i < terminal->line_count; ++i)
        text(terminal, terminal->lines[i],
        10 + 8 * 3 * (i == terminal->actual_ligne), 10 + i * 18);
    return OK;
}

static int cursor(terminal_buffer_t *terminal)
{
    sfRectangleShape *cursor = NULL;
    sfTime time = {0};

    if (!terminal)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    time = sfClock_getElapsedTime(terminal->cursor_clock);
    if ((time.microseconds / 500000) % 2 == 0) {
        cursor = sfRectangleShape_create();
        sfRectangleShape_setSize(cursor, (sfVector2f){8, 13});
        sfRectangleShape_setFillColor(cursor, sfGreen);
        sfRectangleShape_setPosition(cursor, (sfVector2f){2.5 + 8 * 3 *
        (terminal->actual_ligne == terminal->line_count) + 8 +
        get_size(terminal->current_line), 10 + terminal->line_count * 18});
        sfRenderWindow_drawRectangleShape(terminal->window, cursor, NULL);
        sfRectangleShape_destroy(cursor);
    }
    return OK;
}

int display_pty(main_data_t *data)
{
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    sfRenderWindow_clear(data->terminal->window, sfBlack);
    if (diplay_past_line(data->terminal) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (diplay_actual_line(data->terminal) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    text(data->terminal, data->terminal->current_line, 10 + 8 * 3 *
    (data->terminal->actual_ligne == data->terminal->line_count),
    10 + data->terminal->line_count * 18);
    if (cursor(data->terminal) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    sfRenderWindow_display(data->terminal->window);
    return OK;
}
