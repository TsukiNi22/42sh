/*
** EPITECH PROJECT, 2025
** const.c
** File description:
** Define global const variable
*/

#include "minishell.h"

char const *rc_ligne[] = {
    "# Welcome adventure to the bananarc if you're here i ",
    "should tell you this before anything else:\n",
    "# You know, I don t think there are good or bad descriptions,\n",
    "# for me, life is all about functions...\n",
    "#\n",
    "# The bananarc execute each lines stored in this file ",
    "at the initialisation, like a user input\n",
    NULL
};

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
    "\t-color [r_value | r_value g_value | r_value g_value b_value]",
    "\t(default value: 255)\n",
    "\t-alias [alias_name | alias_name alias_value]\n",
    "\t-unalias [alias_name]+\n",
    "\t-history [number_to_diplay]",
    "\t(default value: all history)\n",
    "\t-source [-c | --create | file_name]\n",
    "\t-rehash\n",
    "\t-set [var_name | var_name var_value]\n",
    "\t-unset [var_name]+\n",
    "\t-clear\n",
    "\t-which [command | alias | builtin]+\n",
    "\t-where [command | alias | builtin]+\n",
    "\t-.SILENT [true | false]\n",
    NULL
};

char const *builtin_array_name[] = {
    "exit",
    "cd",
    "setenv",
    "unsetenv",
    "set",
    "unset",
    "env",
    "color",
    "help",
    "alias",
    "unalias",
    "history",
    "source",
    "rehash",
    "clear",
    "which",
    "where",
    ".SILENT",
    NULL
};

int (* const builtin_check_syntax[])(main_data_t *, array_t *, int) = {
    &check_syntax_exit,     // exit
    &check_syntax_cd,       // cd
    &check_syntax_setenv,   // setenv
    &check_syntax_unsetenv, // unsetenv
    &check_syntax_set,      // set
    &check_syntax_unset,    // unset
    &check_syntax_env,      // env
    &check_syntax_color,    // color
    &check_syntax_help,     // help
    &check_syntax_alias,    // alias
    &check_syntax_unalias,  // unalias
    &check_syntax_history,  // history
    &check_syntax_source,   // source
    &check_syntax_rehash,   // rehash
    &check_syntax_clear,    // clear
    &check_syntax_which,    // which
    &check_syntax_where,    // where
    &check_syntax_silent    // .SILENT
};

int (* const builtin_func[])(main_data_t *, array_t *, int) = {
    &builtin_exit,      // exit
    &builtin_cd,        // cd
    &builtin_setenv,    // setenv
    &builtin_unsetenv,  // unsetenv
    &builtin_set,       // set
    &builtin_unset,     // unset
    &builtin_env,       // env
    &builtin_color,     // color
    &builtin_help,      // help
    &builtin_alias,     // alias
    &builtin_unalias,   // unalias
    &builtin_history,   // history
    &builtin_source,    // source
    &builtin_rehash,    // rehash
    &builtin_clear,     // clear
    &builtin_which,     // which
    &builtin_where,     // where
    &builtin_silent     // .SILENT
};
