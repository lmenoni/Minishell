/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_array.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <lmenoni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 18:22:45 by lmenoni           #+#    #+#             */
/*   Updated: 2025/05/20 12:42:12 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    handle_amb_redi(t_flist *file, t_token *tok)
{
    if (tok->next->type != AMB_REDI)
        return ;
    while (file->next)
        file = file->next;
    file->amb_redi = true;
}

void    add_files_to_arr(t_cmd *cmd, t_token **head, t_data *data)
{
    t_token *t;

    t = *head;
    while (t && t->type != PIPE)
    {
        if (t->type >= 3)
        {
            if (t->type == REDI_IN)
                add_file_node(&cmd->files, t->next->s, false, false);
            else if (t->type == REDI_OUT)
                add_file_node(&cmd->files, t->next->s, false, true);
            else if (t->type == HERE_DOC)
                add_file_node(&cmd->files, t->next->s, true, false);
            else if (t->type == APPEND)
                add_file_node(&cmd->files, t->next->s, true, true);
            handle_amb_redi(cmd->files, t);
            remove_two(head, &t, data);
        }
        else
            t = t->next;
    }
}

void    make_cmd_array(t_data *data)
{
    t_token *curr;
    t_cmd   *temp;

    curr = data->token;
    temp = NULL;
    while (curr)
    {
        temp = add_cmd_to_arr(data);
        if (curr->prev && curr->prev->type == PIPE)
            temp->pipe_in = true;
        add_files_to_arr(temp, &curr, data);
        get_args(temp, &curr);
        if (!curr)
            break ;
        if (curr->type == PIPE)
            temp->pipe_out = true;
        curr = curr->next;
    }
}

t_cmd    *add_cmd_to_arr(t_data *data)
{
    t_cmd   *temp;
    int     i;

    i = 0;
    temp = NULL;
    if (!(data->cmd_arr))
    {
        data->cmd_arr = malloc(1 * sizeof(t_cmd));
        data->cmd_arr[0] = (t_cmd){0};
        data->cmd_count++;
        return (&(data->cmd_arr[0]));
    }
    temp = malloc((data->cmd_count + 1) * sizeof(t_cmd));
    while (i < data->cmd_count)
    {
        temp[i] = data->cmd_arr[i];
        i++;
    }
    free(data->cmd_arr);
    temp[i] = (t_cmd){0};
    data->cmd_arr = temp;
    data->cmd_count++;
    return (&(data->cmd_arr[i]));
}
