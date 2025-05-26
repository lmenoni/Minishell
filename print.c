/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <lmenoni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 17:53:43 by lmenoni           #+#    #+#             */
/*   Updated: 2025/05/20 12:47:48 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    print_env(t_env *node)
{
    while (node)
    {
        ft_printf("%s\n", node->e);
        node = node->next;
    }
}

void    print_tokens(t_token *token)
{
    while (token)
    {
        ft_printf("String is (%s) of tipe (%d) ", token->s, token->type);
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
        printf("%s (%s)(%d)",type, t->s, t->x_factor);
        if (t->next)
            printf(", ");
        t = t->next;
    }
    printf("\n");
}