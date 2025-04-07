/*
** EPITECH PROJECT, 2025
** color.c
** File description:
** Builtin function color
*/

#include "define.h"
#include "write.h"
#include "my_string.h"
#include "minishell.h"
#include "error.h"

int builtin_color(main_data_t *data, array_t *input, int start)
{
    if (!data || !input)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    data->r = 255;
    data->g = 255;
    data->b = 255;
    if (input->len - start >= 2)
        data->r = my_atoi(input->data[start + 1]);
    if (input->len - start >= 3)
        data->g = my_atoi(input->data[start + 2]);
    if (input->len - start == 4)
        data->b = my_atoi(input->data[start + 3]);
    if (!data->silent &&
        my_printf("Color set to: %d, %d, %d\n%CThis is a color test :)\n%R",
        data->r, data->g, data->b, data->r, data->g, data->b) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}
