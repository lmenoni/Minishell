/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_mem.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <lmenoni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:45:21 by lmenoni           #+#    #+#             */
/*   Updated: 2025/05/09 18:13:36 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    free_token(t_token *token)
{
    t_token *temp;

    while (token)
    {
        temp = token;
        token = token->next;
        free(temp->s);
        free(temp);
    }
}

void    free_flist(t_flist *flist)
{
    t_flist *temp;

    while (flist)
    {
        temp = flist;
        flist = flist->next;
        free(temp->s);
        free(temp);
    }
}

void    free_cmd_array(t_data *data)
{
    int i;

    i = 0;
    while (i < data->cmd_count)
    {
        free_flist(data->cmd_arr[i].input);
        free_flist(data->cmd_arr[i].output);
        free(data->cmd_arr[i].args);
        i++;
    }
    free(data->cmd_arr);
}
