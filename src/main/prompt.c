/*
** EPITECH PROJECT, 2025
** prompt.c
** File description:
** You know, I don t think there are good or bad descriptions,
** for me, life is above all about functions...
*/

#include "my_string.h"
#include "memory.h"
#include "file.h"
#include "define.h"
#include "write.h"
#include "minishell.h"
#include "error.h"
#include <stdlib.h>
#include <stdbool.h>

static int set_relative_path(main_data_t *data)
{
    char *home_path = NULL;
    int i = 0;

    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (data->relative_path)
        free(data->relative_path);
    if (my_malloc_c(&data->relative_path, my_strlen(data->abs_path) + 1) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    home_path = ht_search(data->env, "HOME");
    if (home_path && my_strstr(data->abs_path, home_path)) {
        for (i = 0; data->abs_path[i] && home_path[i]
        && data->abs_path[i] == home_path[i]; i++);
        i += (data->abs_path[i] && data->abs_path[i] == '/');
        if (!my_strcat(data->relative_path, "~/"))
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    if (!my_strcat(data->relative_path, &(data->abs_path[i])))
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}

static int prompt_github(void)
{
    char *file = get_file(GIT_FILE);
    char *branch = NULL;
    int i = 0;

    if (!file)
        return OK;
    branch = my_strstr(file, "ref: refs/heads/");
    if (!branch || my_strlen(branch) < 16)
        return OK;
    branch = &(branch[16]);
    for (i = 0; branch[i] && branch[i] != '\n'; i++);
    branch[i] = '\0';
    if (my_printf("%R%S: %C%s", 150, 75, 0, branch) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    free(file);
    return OK;
}

int set_prompt(main_data_t *data)
{
    bool c = true;
    int res = 0;

    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (set_relative_path(data) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    c = (data->return_value == OK);
    res += my_printf("%R%S%C$%R%S[%.3d] %C%s", 255 * (!c), 255 * c, 0,
    data->return_value, 0, 200, 200, data->relative_path);
    res += prompt_github();
    res += my_printf("%R➤  %C", data->r, data->g, data->b);
    if (res == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}
