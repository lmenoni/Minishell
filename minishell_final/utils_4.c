/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 15:08:00 by lmenoni           #+#    #+#             */
/*   Updated: 2025/06/09 18:58:20 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    create_pipe_arr(t_data *data)
{
    int i;

    i = 0;
    if (data->cmd_count > 1)
    {
        data->pipe = malloc((data->cmd_count - 1) * sizeof(int *));
        while (i < (data->cmd_count - 1))
        {
            data->pipe[i] = malloc(2 * sizeof(int));
            pipe(data->pipe[i]);
            i++;   
        }
    }
}

void    free_data(t_data *data)
{
    if (data->input)
        free(data->input);
    if (data->token)
        free_token(data->token);
    if (data->cmd_arr)
        free_cmd_array(data);
    if (data->env_data)
        free_env(data->env_data);
    if (data->current_path)
        free(data->current_path);
    if (data->pipe)
        free_pipe(data->pipe, (data->cmd_count - 1));
    if (data->old_path)
        free(data->old_path);
    close(data->st_in);
    close(data->st_out);
}

void    free_all(t_data *data, t_cmd *cmd)
{
    close_if(cmd, data);
    if (cmd->path)
        free(cmd->path);
    free_data(data);
}

bool    no_more_input(t_flist *t)
{
    t_flist *curr;

    curr = t->next;
    while (curr)
    {
        if (!(curr->io_bool))
            return (false);
        curr = curr->next;
    }
    return (true);
}

bool    is_in_pipe(int fd, int **pipe, t_data *data)
{
    int i;

    i = 0;
    while (i < (data->cmd_count - 1))
    {
        if (fd == pipe[i][0] || fd == pipe[i][1])
            return (true);
        i++;
    }
    return (false);
}
