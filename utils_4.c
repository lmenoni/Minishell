/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 15:08:00 by lmenoni           #+#    #+#             */
/*   Updated: 2025/06/05 15:08:03 by lmenoni          ###   ########.fr       */
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
    if (data->curr_path)
        free(data->curr_path);
    if (data->pipe)
        free_pipe(data->pipe, (data->cmd_count - 1));
}

void    free_all(t_data *data, t_cmd *cmd)
{
    if (cmd->in_fd != -1  && cmd->in_fd != 0
        && !is_in_pipe(cmd->in_fd, data->pipe, data))
        close(cmd->in_fd);
    if (cmd->ou_fd != -1  && cmd->ou_fd != 0
        && !is_in_pipe(cmd->ou_fd, data->pipe, data))
        close(cmd->ou_fd);
    if (cmd->path)
        free(cmd->path);
    free_data(data);
}

char    **ft_matdup(char **mat)
{
    int len;
    int i;
    char    **r;

    len = ft_matlen(mat);
    i = 0;
    r = malloc((len + 1) * sizeof(char *));
    if (!r)
        return (NULL);
    while (i < len)
    {
        r[i] = ft_strdup(mat[i]);
        i++;
    }
    r[i] = NULL;
    return (r);
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
