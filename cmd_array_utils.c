/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_array_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <lmenoni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 18:27:45 by lmenoni           #+#    #+#             */
/*   Updated: 2025/05/20 12:43:34 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    remove_token(t_token **t)
{
    t_token *temp;

    temp = (*t);
    (*t) = (*t)->next;
    if (temp && temp->type >= 2)
        free(temp->s);
    free(temp);
}

void    remove_two(t_token **head, t_token **t, t_data *data)
{
    if ((*t) == (*head))
    {
        (*head) = (*t)->next->next;
        if (!(*t)->prev)
        {  
            if (*head)
                (*head)->prev = NULL;
            data->token = (*head);
        }
        else
        {
            (*t)->prev->next = (*head);
            (*head)->prev = (*t)->prev;
        }
        remove_token(t);
        remove_token(t);
        return ;
    }
    (*t)->prev->next = (*t)->next->next;
    if ((*t)->prev->next)
        (*t)->next->next->prev = (*t)->prev;
    remove_token(t);
    remove_token(t);
}

void    add_file_node(t_flist **head, char *s, bool x, bool io)
{
    t_flist *new;
    t_flist *temp;

    new = malloc(1 * sizeof(t_flist));
    new->s = s;
    new->x_factor = x;
    new->io_bool = io;
    new->next = NULL;
    if (!(*head))
    {
        (*head) = new;
        return ;
    }
    temp = (*head);
    while (temp->next)
        temp = temp->next;
    temp->next = new;
}

void    get_args(t_cmd *cmd, t_token **curr)
{
    int i;
    t_token *temp;
    
    temp = *curr;
    i = 0;
    while (temp && temp->type != PIPE && temp->type < 2)
    {
        temp = temp->next;
        i++;
    }
    cmd->args = malloc((i + 1) * sizeof(char *));
    i = 0;
    while(*curr && (*curr)->type != PIPE && (*curr)->type < 2)
    {
        cmd->args[i] = (*curr)->s;
        *curr = (*curr)->next;
        i++;
    }
    cmd->args[i] = NULL;
}