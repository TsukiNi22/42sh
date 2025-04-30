#include <SFML/Graphics.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <pty.h>

#define MAX_LINES 30
#define MAX_LINE_LENGTH 90
#define MAX_LINE_SIZE (MAX_LINE_LENGTH * 8)

typedef struct {
    char lines[MAX_LINES][MAX_LINE_LENGTH + 1];
    int last_apartenance;
    int apartenance[MAX_LINES];
    int line_count;
    int actual_ligne;
} LineBuffer;

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

static void prompt(sfRenderWindow *window, sfFont *font, int i)
{
    sfText* text = sfText_create();
    sfText_setString(text, "$> ");
    sfText_setFont(text, font);
    sfText_setCharacterSize(text, 13);
    sfText_setPosition(text, (sfVector2f){10, 10 + i * 18});
    sfText_setColor(text, sfWhite);
    sfRenderWindow_drawText(window, text, NULL);
    sfText_destroy(text);
}

static void display(sfRenderWindow *window, sfFont *font, sfClock *cursor_clock, LineBuffer buffer, char *current_line)
{
    // Clear window
    sfRenderWindow_clear(window, sfBlack);

    // Affiche toutes les lignes précédentes
    int apartenance = 0;
    bool new = false;
    for (int i = 0; i < buffer.actual_ligne; ++i) {
        if ((i == 0 || apartenance != buffer.apartenance[i]) && buffer.apartenance[i] != -1) {
            prompt(window, font, i);
            apartenance = buffer.apartenance[i];
            new = true;
        }
        sfText* text = sfText_create();
        sfText_setString(text, buffer.lines[i]);
        sfText_setFont(text, font);
        sfText_setCharacterSize(text, 13);
        sfText_setPosition(text, (sfVector2f){10 + 8 * 3 * new, 10 + i * 18});
        sfText_setColor(text, sfWhite);
        sfRenderWindow_drawText(window, text, NULL);
        sfText_destroy(text);
        new = false;
    }

    // Affiche la ligne en cours d'écriture
    prompt(window, font, buffer.actual_ligne);
    for (int i = buffer.actual_ligne; i < buffer.line_count; ++i) {
        sfText* text = sfText_create();
        sfText_setString(text, buffer.lines[i]);
        sfText_setFont(text, font);
        sfText_setCharacterSize(text, 13);
        sfText_setPosition(text, (sfVector2f){10 + 8 * 3 * (i == buffer.actual_ligne), 10 + i * 18});
        sfText_setColor(text, sfGreen);
        sfRenderWindow_drawText(window, text, NULL);
        sfText_destroy(text);
    }

    // Afficher le buffer actuel
    sfText* text = sfText_create();
    sfText_setString(text, current_line);
    sfText_setFont(text, font);
    sfText_setCharacterSize(text, 13);
    sfText_setPosition(text, (sfVector2f){10 + 8 * 3 * (buffer.actual_ligne == buffer.line_count), 10 + buffer.line_count * 18});
    sfText_setColor(text, sfGreen);
    sfRenderWindow_drawText(window, text, NULL);
    sfText_destroy(text);
    

    // Curseur clignotant
    sfTime time = sfClock_getElapsedTime(cursor_clock);
    if ((time.microseconds / 500000) % 2 == 0) { // clignote toutes les 0.5s
        sfRectangleShape* cursor = sfRectangleShape_create();
        sfRectangleShape_setSize(cursor, (sfVector2f){8, 13});
        sfRectangleShape_setFillColor(cursor, sfGreen);
        sfRectangleShape_setPosition(cursor, (sfVector2f){2.5 + 8 * 3 * (buffer.actual_ligne == buffer.line_count) + 8 + get_size(current_line), 10 + buffer.line_count * 18});
        sfRenderWindow_drawRectangleShape(window, cursor, NULL);
        sfRectangleShape_destroy(cursor);
    }

    sfRenderWindow_display(window);
}

void run_command(char *command, sfRenderWindow *window, sfFont *font, sfClock *cursor_clock, LineBuffer *buffer, char *current_line)
{
    if (!command)
        return;
    if (strncmp(command, "exit", 4) == 0)
        exit(0);
    
    if (strncmp(command, "clear", 5) == 0) {
        memset(buffer->lines, 0, sizeof(char) * (MAX_LINES - 1) * MAX_LINE_LENGTH);
        memset(buffer->apartenance, 0, sizeof(int) * (MAX_LINES - 1));
        buffer->last_apartenance = -1;
        buffer->line_count = 0;
        buffer->actual_ligne = 0;
        return;
    }

    pid_t pid;
    char *args[100]; // tableau d'arguments (max 9 mots + NULL)
    int i = 0;
    char *token;

    // Split la commande (très simple split sur les espaces)
    token = strtok(command, " \t");
    while (token && i < 100) {
        args[i++] = token;
        token = strtok(NULL, " \t");
    }
    args[i] = NULL; // très important : terminer le tableau par NULL

    int master_fd;
    struct winsize ws = {30, 90, 0, 0};

    pid = forkpty(&master_fd, NULL, NULL, &ws);
    if (pid < 0) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) {
        // Fils : on exécute
        execvp(args[0], args); // execvp cherche dans $PATH
        perror("execvp");
        _exit(1);
    } else {
        char str[1024];
        int nbytes;
        int current_pos = 0;

        while ((nbytes = read(master_fd, str, sizeof(str))) > 0) {
            sfEvent event;
            while (sfRenderWindow_pollEvent(window, &event)) {
                if (event.type == sfEvtClosed) {
                    sfRenderWindow_close(window);
                } else if (event.type == sfEvtTextEntered) {
                    char c = event.text.unicode;
                    if (c == 3 || c == 'q') // ctrl + c
                        kill(pid, SIGKILL);
                    else if (c == 4) // Ctrl+D, EOF (peut être ajusté pour SIGTERM)
                        kill(pid, SIGTERM);
                }
            }
            
            for (int i = 0; i < nbytes; ++i) {
                char c = str[i];
         
                if (c == '\n' || get_size(current_line) >= MAX_LINE_SIZE - 8) {

                    // Terminer la ligne actuelle
                    current_line[current_pos] = '\0';
         
                    // Si trop de lignes, on supprime la plus ancienne
                    if (buffer->line_count >= MAX_LINES) {
                        memmove(buffer->lines, buffer->lines + 1, (MAX_LINES - 1) * MAX_LINE_LENGTH);
                        memmove(buffer->apartenance, buffer->apartenance + 1, (MAX_LINES - 1) * sizeof(int));
                        buffer->line_count--;
                    }
         
                    buffer->apartenance[buffer->line_count] = -1;
                    strcpy(buffer->lines[buffer->line_count++], current_line);
         
                    // Reset pour la prochaine ligne
                    current_pos = 0;
                    memset(current_line, 0, sizeof(current_line));
                    buffer->actual_ligne = buffer->line_count;
                } else {
                    if (get_size(current_line) < MAX_LINE_SIZE - 8) {
                        if (!((c >= 32 && c <= 126) || (c >= 7 && c <= 13))) {
                            c = -80;
                        }
                        current_line[current_pos++] = c;
                        current_line[current_pos] = '\0';
                    } else {
                        current_line[current_pos] = '\0';
                        if (buffer->line_count >= MAX_LINES) {
                            memmove(buffer->lines, buffer->lines + 1, (MAX_LINES - 1) * MAX_LINE_LENGTH);
                            memmove(buffer->apartenance, buffer->apartenance + 1, (MAX_LINES - 1) * sizeof(int));
                            buffer->line_count--;
                            buffer->actual_ligne--;
                        }
                        buffer->apartenance[buffer->line_count] = -1;
                        strcpy(buffer->lines[buffer->line_count++], current_line);
                        current_pos = 0;
                        memset(current_line, 0, sizeof(current_line));
                    }
                }
            }
            
            // Afficher la fenêtre
            display(window, font, cursor_clock, *buffer, current_line);
        }

        if (strlen(current_line) > 0) {
             current_line[current_pos] = '\0';
             
             // Si trop de lignes, on supprime la plus ancienne
             if (buffer->line_count >= MAX_LINES) {
                 memmove(buffer->lines, buffer->lines + 1, (MAX_LINES - 1) * MAX_LINE_LENGTH);
                 memmove(buffer->apartenance, buffer->apartenance + 1, (MAX_LINES - 1) * sizeof(int));
                 buffer->line_count--;
             }
             
             buffer->apartenance[buffer->line_count] = -1;
             strcpy(buffer->lines[buffer->line_count++], current_line);
             
             int len = strlen(buffer->lines[buffer->line_count - 1]);
             buffer->lines[buffer->line_count - 1][len] = '%';
             buffer->lines[buffer->line_count - 1][len + 1] = '\0';
             
             // Reset pour la prochaine ligne
             current_pos = 0;
             memset(current_line, 0, sizeof(current_line));
             buffer->actual_ligne = buffer->line_count;
        }

        // Parent : attend
        int status = 0;
        waitpid(pid, &status, WUNTRACED);
    }
}

char *cat_str(char *first, char *second)
{
    char *new_str;
    size_t len_first = first ? strlen(first) : 0;
    size_t len_second = second ? strlen(second) : 0;

    new_str = malloc(len_first + len_second + 1);
    if (!new_str)
        return NULL;
    
    if (first)
        memcpy(new_str, first, len_first);
    if (second)
        memcpy(new_str + len_first, second, len_second);

    new_str[len_first + len_second] = '\0';

    if (first)
        free(first);
    
    return new_str;
}

int main()
{    
    // SFML setup
    sfRenderWindow* window;
    sfVideoMode mode = {800, 600, 32};
    window = sfRenderWindow_create(mode, "Banana and Shell Handler", sfClose, NULL);
    if (!window)
        return 1;

    sfFont* font = sfFont_createFromFile("/usr/share/fonts/liberation-mono/LiberationMono-Regular.ttf");
    if (!font) {
        printf("Cannot load font.\n");
        sfRenderWindow_destroy(window);
        return 1;
    }

    LineBuffer buffer = { .last_apartenance = -1, .line_count = 0, .actual_ligne = 0};
    char current_line[MAX_LINE_LENGTH] = {0};
    int current_pos = 0;
    sfClock* cursor_clock = sfClock_create();

    while (sfRenderWindow_isOpen(window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            } else if (event.type == sfEvtTextEntered) {
                char c = event.text.unicode;
                if (c == 13) { // \n
                    current_line[current_pos] = '\0';
                    if (buffer.line_count >= MAX_LINES) {
                        memmove(buffer.lines, buffer.lines + 1, (MAX_LINES - 1) * MAX_LINE_LENGTH);
                        memmove(buffer.apartenance, buffer.apartenance + 1, (MAX_LINES - 1) * sizeof(int));
                        buffer.line_count--;
                    }
                    buffer.apartenance[buffer.line_count] = buffer.last_apartenance + 1;
                    strcpy(buffer.lines[buffer.line_count++], current_line);
                    buffer.last_apartenance++;
                    current_pos = 0;
                    memset(current_line, 0, sizeof(current_line));
                    buffer.actual_ligne = buffer.line_count;

                    char *cmd = NULL;
                    for (int i = 0; i < MAX_LINES; i++) {
                        if (buffer.apartenance[i] == buffer.last_apartenance)
                            cmd = cat_str(cmd, buffer.lines[i]);
                    }
                    if (cmd)
                        run_command(cmd, window, font, cursor_clock, &buffer, current_line);
                } else if (c == 127 || c == 8) { // Backspace
                    if (current_pos > 0) {
                        current_pos--;
                        current_line[current_pos] = '\0';
                    } else if (buffer.actual_ligne < buffer.line_count) {
                        buffer.line_count--;
                        for (current_pos = 0; buffer.lines[buffer.line_count][current_pos]; current_pos++)
                            current_line[current_pos] = buffer.lines[buffer.line_count][current_pos];
                        memset(buffer.lines[buffer.line_count], 0, sizeof(char) * MAX_LINE_LENGTH);
                    }
                } else if (c == 9 || c >= 32 && c <= 126) { // Caractères imprimables + tab
                    if (get_size(current_line) >= MAX_LINE_SIZE - 8 && buffer.line_count - buffer.actual_ligne >= MAX_LINES)
                        continue;
                    if (get_size(current_line) < MAX_LINE_SIZE - 8) {
                        current_line[current_pos++] = c;
                        current_line[current_pos] = '\0';
                    } else {
                        current_line[current_pos] = '\0';
                        if (buffer.line_count >= MAX_LINES) {
                            memmove(buffer.lines, buffer.lines + 1, (MAX_LINES - 1) * MAX_LINE_LENGTH);
                            memmove(buffer.apartenance, buffer.apartenance + 1, (MAX_LINES - 1) * sizeof(int));
                            buffer.line_count--;
                            buffer.actual_ligne--;
                        }
                        buffer.apartenance[buffer.line_count] = buffer.last_apartenance + 1;
                        strcpy(buffer.lines[buffer.line_count++], current_line);
                        current_pos = 0;
                        memset(current_line, 0, sizeof(current_line));
                    }
                }
            }
        }

        display(window, font, cursor_clock, buffer, current_line);
    }

    // Cleanup
    sfFont_destroy(font);
    sfRenderWindow_destroy(window);
    sfClock_destroy(cursor_clock);
    return 0;
}
