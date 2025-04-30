/*
** EPITECH PROJECT, 2025
** pty_handler.c
** File description:
** Handle the input for the pty
*/

#include "memory.h"
#include "my_string.h"
#include "minishell.h"
#include "error.h"
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SFML/Graphics.h>

static int get_size(char *line)
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

static char *cat_str(char *first, char *second)
{
    char *new_str = NULL;
    int len_first = my_strlen(first);
    int len_second = my_strlen(second);

    if (len_first == KO && len_second == KO)
        return err_prog_n(PTR_ERR, ERR_INFO);
    len_first = len_first * (len_first != KO);
    len_second = len_second * (len_second != KO);
    if (my_malloc_c(&new_str, len_first + len_second + 1) == KO)
        return err_prog_n(UNDEF_ERR, ERR_INFO);
    if (first)
        memcpy(new_str, first, len_first);
    if (second)
        memcpy(new_str + len_first, second, len_second);
    new_str[len_first + len_second] = '\0';
    if (first)
        free(first);
    return new_str;
}

static int back_line(terminal_buffer_t *terminal, char **str, bool exe)
{
    char *cmd = NULL;

    if (!terminal)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    terminal->current_line[terminal->current_pos] = '\0';
    if (terminal->line_count >= MAX_LINES) {
        memmove(terminal->lines, terminal->lines + 1, (MAX_LINES - 1) * MAX_LINE_LENGTH);
        memmove(terminal->apartenance, terminal->apartenance + 1, (MAX_LINES - 1) * sizeof(int));
        terminal->line_count--;
    }
    terminal->apartenance[terminal->line_count] = terminal->last_apartenance + 1;
    strcpy(terminal->lines[terminal->line_count++], terminal->current_line);
    terminal->last_apartenance++;
    terminal->current_pos = 0;
    memset(terminal->current_line, 0, sizeof(terminal->current_line));
    terminal->actual_ligne = terminal->line_count;
    if (exe) {
        for (int i = 0; exe && i < MAX_LINES; i++) {
            if (terminal->apartenance[i] == terminal->last_apartenance)
                cmd = cat_str(cmd, terminal->lines[i]);
        }
        *str = cmd;
    }
    return OK;
}

static int backspace(terminal_buffer_t *terminal)
{
    if (!terminal)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (terminal->current_pos > 0) {
        terminal->current_pos--;
        terminal->current_line[terminal->current_pos] = '\0';
    } else if (terminal->actual_ligne < terminal->line_count) {
        terminal->line_count--;
        for (terminal->current_pos = 0; terminal->lines[terminal->line_count][terminal->current_pos]; terminal->current_pos++)
            terminal->current_line[terminal->current_pos] = terminal->lines[terminal->line_count][terminal->current_pos];
        memset(terminal->lines[terminal->line_count], 0, sizeof(char) * MAX_LINE_LENGTH);
    }
    return OK;
}

static int add_char(terminal_buffer_t *terminal, char c)
{
    if (!terminal)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (get_size(terminal->current_line) < MAX_LINE_SIZE - 8) {
        if (!((c >= 32 && c <= 126) || (c >= 7 && c <= 13)))
            c = -80;
        terminal->current_line[terminal->current_pos++] = c;
        terminal->current_line[terminal->current_pos] = '\0';
    } else {
        terminal->current_line[terminal->current_pos] = '\0';
        if (terminal->line_count >= MAX_LINES) {
            memmove(terminal->lines, terminal->lines + 1, (MAX_LINES - 1) * MAX_LINE_LENGTH);
            memmove(terminal->apartenance, terminal->apartenance + 1, (MAX_LINES - 1) * sizeof(int));
            terminal->line_count--;
            terminal->actual_ligne--;
        }
        terminal->apartenance[terminal->line_count] = terminal->last_apartenance + 1;
        strcpy(terminal->lines[terminal->line_count++], terminal->current_line);
        terminal->current_pos = 0;
        memset(terminal->current_line, 0, sizeof(terminal->current_line));
    }
    return OK;
}

int pty_input_char(terminal_buffer_t *terminal, char **str, char c, bool exe)
{
    if (!terminal || !str)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (c == 13) { // \n
        back_line(terminal, str, exe);
        return 2;
    } else if (c == 127 || c == 8) { // Backspace
        backspace(terminal);
    } else { // Caractères imprimables + tab
        if (get_size(terminal->current_line) >= MAX_LINE_SIZE - 8 && terminal->line_count - terminal->actual_ligne >= MAX_LINES)
            return OK;
        add_char(terminal, c);
    }
    return OK;
}

int pty_input_handler(main_data_t *data)
{
    sfEvent event;
    char c = '\0';

    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    memset(data->terminal->current_line, 0, sizeof(data->terminal->current_line));
    data->terminal->current_pos = 0;
    while (sfRenderWindow_isOpen(data->terminal->window)) {
        while (sfRenderWindow_pollEvent(data->terminal->window, &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(data->terminal->window);
            if (event.type == sfEvtTextEntered) {
                c = event.text.unicode;
                if (pty_input_char(data->terminal, &data->input, c, true) == 2)
                    return OK;
            }
        }
        if (sfRenderWindow_isOpen(data->terminal->window))
            display_pty(data);
    }
    data->out = true;
    return OK;
}
