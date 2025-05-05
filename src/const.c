/*
** EPITECH PROJECT, 2025
** const.c
** File description:
** Define global const variable
*/

#include "minishell.h"

char const *help_ligne[] = {
    "Usage:\n",
    "\tbinary_path [argument]+\n",
    "\tbuiltin [argument]+\n\n",
    "Builtin:\n",
    "\t-exit [exit_value]\n",
    "\t-cd [path | -]\n",
    "\t-env\n",
    "\t-setenv [var_name | var_name var_value]\n",
    "\t-unsetenv [var_name]+",
    "\t(doesn't support the '*' wildcard)\n\n",
    "Builtin Bonus:\n",
    "\t-help\n",
    "\t-color [r_value | r_value g_value | r_value g_value b_value]\n",
    "\t-alias [alias_name | alias_name alias_value]\n",
    "\t-unalias alias_name1 [alias_name2 ...]\n",
    "\t-history [number_to_diplay]\n",
    "\t-source [-c | --create | file_name]\n",
    "\t-rehash\n",
    "\t-set [var_name | var_name var_value]\n",
    "\t-unset [var_name]+\n",
    "\t-.SILENT [true | false]\n",
    NULL
};

char const *builtin_array_name[] = {
    "exit",
    "cd",
    "setenv",
    "unsetenv",
    "env",
    "color",
    "help",
    "alias",
    "unalias",
    "history",
    "source",
    "rehash",
    ".SILENT",
    NULL
};

int (* const builtin_check_syntax[])(main_data_t *, array_t *, int) = {
    &check_syntax_exit,     // exit
    &check_syntax_cd,       // cd
    &check_syntax_setenv,   // setenv
    &check_syntax_unsetenv, // unsetenv
    &check_syntax_env,      // env
    &check_syntax_color,    // color
    &check_syntax_help,     // help
    &check_syntax_alias,    // alias
    &check_syntax_unalias,  // unalias
    &check_syntax_history,  // history
    &check_syntax_source,   // source
    &check_syntax_rehash,   // rehash
    &check_syntax_silent    // .SILENT
};

int (* const builtin_func[])(main_data_t *, array_t *, int) = {
    &builtin_exit,      // exit
    &builtin_cd,        // cd
    &builtin_setenv,    // setenv
    &builtin_unsetenv,  // unsetenv
    &builtin_env,       // env
    &builtin_color,     // color
    &builtin_help,      // help
    &builtin_alias,     // alias
    &builtin_unalias,   // unalias
    &builtin_history,   // history
    &builtin_source,    // source
    &builtin_rehash,    // rehash
    &builtin_silent     // .SILENT
};
