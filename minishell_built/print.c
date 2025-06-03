/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 17:53:43 by lmenoni           #+#    #+#             */
/*   Updated: 2025/06/03 23:00:48 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    print_tokens(t_token *token)
{
    while (token)
    {
        ft_printf("String is (%s) of tipe (%d)", token->s, token->type);
        //if (token->prev)
        //    ft_printf("Prev string is (%s)", token->prev->s);
        ft_printf("\n");
        token = token->next;
    }
    ft_printf("---------------------------------------\n");
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
        printf("FILES ARE:\n");
        print_files(temp[i].files);
        printf("PIPE IN IS:\n%d\n", temp[i].pipe_in);
        printf("PIPE OUT IS:\n%d\n", temp[i].pipe_out);
        i++;
    }
}

void    print_files(t_flist *list)
{
    t_flist *t = list;
    char *type;
    
    while (t)
    {
        if (t->io_bool == 0)
            type = "input";
        else
            type = "output";
        printf("%s (name: %s)(level: %d)(amb: %d)",type, t->s, t->x_factor, t->amb_redi);
        if (t->next)
            printf(", ");
        t = t->next;
    }
    printf("\n");
}