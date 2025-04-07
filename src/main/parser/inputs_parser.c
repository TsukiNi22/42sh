/*
** EPITECH PROJECT, 2025
** parser.c
** File description:
** Parser the given input
*/

#include "my_string.h"
#include "minishell.h"
#include "error.h"

static int set_lens(main_data_t *data, int lens[])
{
    if (!data || !lens)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    lens[0] = my_strlen(data->conditional_string[0]);
    lens[1] = my_strlen(data->conditional_string[1]);
    lens[2] = my_strlen(data->redirection_string[0]);
    if (lens[0] == KO || lens[1] == KO || lens[2] == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}

static bool is_separator(main_data_t *data, char *input, int lens[], int i)
{
    if (!data || !input || !lens)
        return err_prog(PTR_ERR, false, ERR_INFO);
    return (my_strncmp(&input[i], data->conditional_string[0], lens[0]) == 0 ||
    my_strncmp(&input[i], data->conditional_string[1], lens[1]) == 0 ||
    my_strncmp(&input[i], data->redirection_string[0], lens[2]) == 0);
}

static int input_parser(main_data_t *data, char *input)
{
    array_t *input_array = NULL;
    int lens[3] = {0};
    int i = 0;

    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (set_lens(data, lens) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (add_array(data->inputs, new_array()) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    input_array = data->inputs->data[data->inputs->len - 1];
    for (i = 0; input[i]; i++) {
        if ((i > 0 && input[i - 1] == data->esc_char)
            || !is_separator(data, input, lens, i))
            continue;
        if (parser(data, input_array, &input, &i) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    return parser(data, input_array, &input, &i);
}

static int init_var(main_data_t *data, char **last_ptr, int *len)
{
    if (!data || !last_ptr || !len)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    *last_ptr = data->input;
    *len = my_strlen(data->cmd_separator);
    if (*len == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    data->inputs = new_array();
    if (!data->inputs)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}

int inputs_parser(main_data_t *data)
{
    char *last_ptr = NULL;
    char *ptr = NULL;
    int len = 0;

    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    if (init_var(data, &last_ptr, &len) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    do {
        ptr = my_strstr(last_ptr, data->cmd_separator);
        while (ptr && ptr > data->input && *(ptr - 1) == data->esc_char)
            ptr = my_strstr(ptr + len, data->cmd_separator);
        if (ptr)
            ptr[0] = '\0';
        if (input_parser(data, last_ptr) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        last_ptr = ptr + len;
    } while (ptr);
    return OK;
}
