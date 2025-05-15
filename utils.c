/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <lmenoni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:46:48 by lmenoni           #+#    #+#             */
/*   Updated: 2025/05/10 15:41:45 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_space(char c)
{
    return (c == ' ' || (c >= 9 && c <= 13));
}

void    skip_spaces(char *s, int *i)
{
    while (s[*i] != '\0' && is_space(s[*i]))
    {
        *i = *i + 1;
    }
}
void    print_cmd_array(t_data *data)
{
    int i = 0;
    int j = 0;
    t_cmd *temp = data->cmd_arr;

    while (i < data->cmd_count)
    {
        printf("-------------------------------------\n");
        j = 0;
        printf("ARGS ARE:\n");
        while (temp[i].args[j] != NULL)
        {
            printf("%s\n", temp[i].args[j]);
            j++;
        }
        printf("INPUT FILES ARE:\n");
        print_files(temp[i].input);
        printf("OUTPUT FILES ARE:\n");
        print_files(temp[i].output);
        printf("PIPE IN IS:\n%d\n", temp[i].pipe_in);
        printf("PIPE OUT IS:\n%d\n", temp[i].pipe_out);
        i++;
    }
}

void    print_files(t_flist *list)
{
    t_flist *t = list;
    
    while (t)
    {
        printf("%s %d", t->s, t->x_factor);
        if (t->next)
            printf(", ");
        t = t->next;
    }
    printf("\n");
}