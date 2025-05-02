/*
** EPITECH PROJECT, 2025
** pty_exec_handling.c
** File description:
** Handle the execution for the pty
*/

#include "minishell.h"
#include "error.h"
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <SFML/Graphics.h>
#include <stdbool.h>

int pty_input_char_exec(terminal_buffer_t *terminal, char c)
{
    if (!terminal)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (c == '\n' || get_size(terminal->current_line) >= MAX_LINE_SIZE - 8) {
        terminal->current_line[terminal->current_pos] = '\0';
        if (terminal->line_count >= MAX_LINES) {
            memmove(terminal->lines, terminal->lines + 1, (MAX_LINES - 1) * MAX_LINE_LENGTH);
            memmove(terminal->apartenance, terminal->apartenance + 1, (MAX_LINES - 1) * sizeof(int));
            terminal->line_count--;
        }
        terminal->apartenance[terminal->line_count] = -1;
        strcpy(terminal->lines[terminal->line_count++], terminal->current_line);
        terminal->current_pos = 0;
        memset(terminal->current_line, 0, sizeof(terminal->current_line));
        terminal->actual_ligne = terminal->line_count;
    } else {
        if (get_size(terminal->current_line) < MAX_LINE_SIZE - 8) {
            if (!((c >= 32 && c <= 126) || (c >= 7 && c <= 13))) {
                c = -80;
            }
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
            terminal->apartenance[terminal->line_count] = -1;
            strcpy(terminal->lines[terminal->line_count++], terminal->current_line);
            terminal->current_pos = 0;
            memset(terminal->current_line, 0, sizeof(terminal->current_line));
        }
    }
    return OK;
}

int pty_exec_handling(main_data_t *data, pid_t pid)
{
    sfEvent event;
    char str[1024] = {0};
    int nbytes = 0;
    int len = 0;
    char c = '\0';

    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    memset(data->terminal->current_line, 0, sizeof(data->terminal->current_line));
    data->terminal->current_pos = 0;
    while ((nbytes = read(data->master_fd, str, sizeof(str))) > 0) {
        while (sfRenderWindow_pollEvent(data->terminal->window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(data->terminal->window);
            } else if (event.type == sfEvtTextEntered) {
                c = event.text.unicode;
                if (c == 3)
                    kill(pid, SIGKILL);
                else if (c == 4)
                    kill(pid, SIGTERM);
            }
        }
        for (int i = 0; i < nbytes; ++i)
            pty_input_char_exec(data->terminal, str[i]);
        if (sfRenderWindow_isOpen(data->terminal->window))
            display_pty(data);
    }

    if (strlen(data->terminal->current_line) > 0) {
         data->terminal->current_line[data->terminal->current_pos] = '\0';
         if (data->terminal->line_count >= MAX_LINES) {
             memmove(data->terminal->lines, data->terminal->lines + 1, (MAX_LINES - 1) * MAX_LINE_LENGTH);
             memmove(data->terminal->apartenance, data->terminal->apartenance + 1, (MAX_LINES - 1) * sizeof(int));
             data->terminal->line_count--;
         }
         data->terminal->apartenance[data->terminal->line_count] = -1;
         strcpy(data->terminal->lines[data->terminal->line_count++], data->terminal->current_line);
         len = strlen(data->terminal->lines[data->terminal->line_count - 1]);
         data->terminal->lines[data->terminal->line_count - 1][len] = '%';
         data->terminal->lines[data->terminal->line_count - 1][len + 1] = '\0';
         data->terminal->current_pos = 0;
         memset(data->terminal->current_line, 0, sizeof(data->terminal->current_line));
         data->terminal->actual_ligne = data->terminal->line_count;
    }   
    return OK;
}
