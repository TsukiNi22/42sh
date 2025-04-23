/*
** EPITECH PROJECT, 2025
** minishell.h
** File description:
** Minishell header
*/

#ifndef MINISHELL_H
    #define MINISHELL_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #include "hashtable.h" // hashtable_t
    #include "array.h" // array_t
    #include <stdbool.h> // boolean
    #include <time.h> // time_t
    #include <termios.h> // termios

    //----------------------------------------------------------------//
    /* DEFINE */

    /* myshrc */
    #define MYSHRC_FILE ".myshrc"

    /* prompt */
    #define GIT_FILE ".git/HEAD"

    /* history */
    #define HISTORY_SIZE 50000
    #define HISTORY_FILE ".mysh_history"

    /* builtin */
    #define BUILTIN_MIN EXIT
    #define BUILTIN_MAX SILENT

    /* input handling */
    #define MAX_INPUT_STR 2048
    #define VEOF 4
    #define VSIGINT 3

    //----------------------------------------------------------------//
    /* MACRO */

    /* separator */
    #define IS_SEPARATOR(c) (c <= D_RIGHT && c >= AND)

//----------------------------------------------------------------//
/* TYPEDEF */

/* separator -> no printable char */
typedef enum separator_e {
    UNDEFINED = -1,
    SEPARATOR = 22
} separator_val_t;

/* conditional -> no printable char */
typedef enum conditional_e {
    AND = 23,
    OR = 24
} conditional_t;

/* redirection -> no printable char */
typedef enum redirection_e {
    PIPE = 25,
    D_RIGHT = 26,
    RIGHT = 27,
    D_LEFT = 28,
    LEFT = 29 // Group separator
} redirection_t;

/* builtin */
typedef enum builtin_func_e {
    NONE = -1,
    EXIT = 0,
    CD,
    SETENV,
    UNSETENV,
    ENV,
    COLOR,
    HELP,
    ALIAS,
    UNALIAS,
    HISTORY,
    SOURCE,
    REHASH,
    SILENT
} builtin_func_t;

/* data */
typedef struct main_data_s {
    /* redirect_var */
    int pipefd[2];
    int prevfd;
    int stdin_save;
    int stdout_save;

    /* global_variable */
    bool silent;
    bool out;
    bool input_redirect;

    /* input_var */
    bool builtin;
    bool binary;
    builtin_func_t builtin_val;

    /* alias */
    hashtable_t *alias;

    /* path */
    char *last_path;
    char *abs_path;

    /* prompt */
    char esc_char;
    char *cmd_separator;
    char *conditional_string[2];
    char *redirection_string[5];
    unsigned char return_value;
    char *relative_path;
    int r;
    int g;
    int b;

    /* error */
    bool err_sys;

    /* env */
    bool path_changed;
    time_t *sys_mtime;
    int nb_mtime;
    hashtable_t *env;
    char **env_path;
    hashtable_t *sys_func;

    /* input */
    char *input;
    array_t *inputs;
} main_data_t;

//----------------------------------------------------------------//
/* PROTOTYPE */

/* minishell */
int minishell(main_data_t *data); // Error: KO

/* main */
int set_prompt(main_data_t *data); // Error: KO
int get_input(main_data_t *data); // Error: KO
int add_history(main_data_t *data, hashtable_t *env, char *input); // Error: KO
int do_input(main_data_t *data); // Error: KO

/* input_handling */ // Error: KO
int inputs_parser(main_data_t *data); // Error: KO
int parser(main_data_t *data, array_t *array, char **input, int *i);
int cmd_parser(main_data_t *data, array_t *array, char *input, int i);
int check_syntax(main_data_t *data); // Error: KO
int get_input_type(main_data_t *data, array_t *input); // Error: KO
int replace_var(main_data_t *data);

/* redirection */ // Error: KO
int set_redirection(main_data_t *data, array_t *input, array_t *inputs, int i);
int set_pipe_child(main_data_t *data); // Error: KO
int set_pipe_parent(main_data_t *data, array_t *input, int i); // Error: KO
int clear_redirection(main_data_t *data, array_t *input, int i); // Error: KO

/* exec */ // Error: KO
int exe_cmd(main_data_t *data, array_t *cmd, array_t *input, int i);
int exe_input(main_data_t *data, array_t *input); // Error: KO
int exe_inputs(main_data_t *data); // Error: KO

/* redirection */
int get_heredoc(main_data_t *data, char *stop, int *pipefd); // Error: KO

/* usefull */
bool is_accesible_dir(char const *path); // Error: false
bool is_valid_char(char const c); // Error: None
bool is_valid_str(char const *str); // Error: None

/* init_data */
int init_data(main_data_t *data); // Error: KO
int init_global(main_data_t *data); // Error: KO
int init_prompt(main_data_t *data); // Error: KO
int init_env(main_data_t *data); // Error: KO
int init_bonus(main_data_t *data); // Error: KO

/* signal */
void handle_sigsegv(int sig); // Error: None
void handle_sigint(int sig); // Error: None
void handle_sigtstp(int sig); // Error: None
void handle_sigquit(int sig); // Error: None
void set_signal(void);

/* update */
int sys_func_update(main_data_t *data); // Error: KO

/* check_syntax */ // Error: KO
int check_syntax_bonus(main_data_t *data, array_t *input, int start);
int check_syntax_exit(main_data_t *data, array_t *input, int start);
int check_syntax_cd(main_data_t *data, array_t *input, int start);
int check_syntax_setenv(main_data_t *data, array_t *input, int start);
int check_syntax_unsetenv(main_data_t *data, array_t *input, int start);
int check_syntax_env(main_data_t *data, array_t *input, int start);
int check_syntax_color(main_data_t *data, array_t *input, int start);
int check_syntax_help(main_data_t *data, array_t *input, int start);
int check_syntax_alias(main_data_t *data, array_t *input, int start);
int check_syntax_unalias(main_data_t *data, array_t *input, int start);
int check_syntax_history(main_data_t *data, array_t *input, int start);
int check_syntax_source(main_data_t *data, array_t *input, int start);
int check_syntax_rehash(main_data_t *data, array_t *input, int start);
int check_syntax_silent(main_data_t *data, array_t *input, int start);

/* builtin_func */ // Error: KO
int builtin_bonus(main_data_t *data, array_t *input, int start);
int builtin_exit(main_data_t *data, array_t *input, int start);
int builtin_cd(main_data_t *data, array_t *input, int start);
int builtin_setenv(main_data_t *data, array_t *input, int start);
int builtin_unsetenv(main_data_t *data, array_t *input, int start);
int builtin_env(main_data_t *data, array_t *input, int start);
int builtin_color(main_data_t *data, array_t *input, int start);
int builtin_help(main_data_t *data, array_t *input, int start);
int builtin_alias(main_data_t *data, array_t *input, int start);
int builtin_unalias(main_data_t *data, array_t *input, int start);
int builtin_history(main_data_t *data, array_t *input, int start);
int builtin_source(main_data_t *data, array_t *input, int start);
int builtin_rehash(main_data_t *data, array_t *input, int start);
int builtin_silent(main_data_t *data, array_t *input, int start);

/* exit */
int free_data(main_data_t *data); // Error: EPITECH_ERR
int free_array(char **array); // Error: KO
int free_input(void *input); // Error: KO

/* autocompletion */
void suggest(char **prefix, int pos, main_data_t *data);

/* input */
int input_handler(main_data_t *data);

/* signals */
size_t sigint(size_t write, size_t value);

/* prompt */
void print_prompt(main_data_t *data, char *str);

//----------------------------------------------------------------//
/* GLOBAL_CONST */

/* help_builtin */
extern char const *help_ligne[];

/* parser */
extern char const *parser_separator[];

/* builtin_function */
extern char const *builtin_array_name[];
extern int (* const builtin_check_syntax[])(main_data_t *,
    array_t *input, int);
extern int (* const builtin_func[])(main_data_t *,
    array_t *input, int);

#endif /* MINISHELL_H */
