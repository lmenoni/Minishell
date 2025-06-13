/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fds.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 15:07:46 by lmenoni           #+#    #+#             */
/*   Updated: 2025/06/10 18:08:17 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     create_temp_file(char *content, t_data *data)
{
    char    *name;
    char    *path;
    int     fd;

    fd = 0;
    name = ft_itoa(data->cmd_count);
    path = ft_strjoin(".tmp_hd_n", name);
    fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0700);
    if (fd < 0)
        return (free(name), free(path), fd);
    ft_putstr_fd(content, fd);
    close(fd);
    fd = open(path, O_RDONLY);
    unlink(path);
    return (free(name), free(path), fd);
}

void    set_pipe(t_cmd *cmd, t_data *data)
{
    if (data->cmd_name > 0)
        cmd->in_fd = data->pipe[data->cmd_name - 1][0];
    if (data->cmd_name < (data->cmd_count - 1))
        cmd->ou_fd = data->pipe[data->cmd_name][1];
}

bool    open_out(t_flist *t, t_cmd *cmd)
{
    int fd;

    fd = 1;
    if (t->x_factor)
        fd = open(t->s, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else
        fd = open(t->s, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
        return (ft_printf_fd(2, "minishell: %s: ", t->s), perror(""), false);
    cmd->ou_fd = fd;
    return (true);
}

bool    open_in(t_flist *t, t_cmd *cmd, t_data *data)
{
    int fd;

    fd = 0;
    if (t->x_factor && no_more_input(t))
        fd = create_temp_file(t->s, data);
    else if (!t->x_factor)
        fd = open(t->s, O_RDONLY);
    if (fd < 0)
        return (ft_printf_fd(2, "minishell: %s: ", t->s), perror(""), false);
    cmd->in_fd = fd;
    return (true);
}

bool    do_open(t_cmd *cmd, t_data *data)
{
    t_flist *t;

    t = cmd->files;
    while(t)
    {
        if (t->io_bool == 0 && cmd->in_fd != data->st_in && !is_in_pipe(cmd->in_fd, data->pipe, data))
            close(cmd->in_fd);
        if (t->io_bool == 1 && cmd->ou_fd != data->st_out && !is_in_pipe(cmd->ou_fd, data->pipe, data))
            close(cmd->ou_fd);
        if (t->amb_redi)
            return (ft_printf_fd(2, "minishell: %s: ambiguous redirect\n", t->s), false);
        if (t->io_bool)
        {
            if (!open_out(t, cmd))
                return (false);
        }
        else
        {
            if (!open_in(t, cmd, data))
                return (false);
        }
        t = t->next;
    }
    return (true);
}
