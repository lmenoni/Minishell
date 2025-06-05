/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 15:07:32 by lmenoni           #+#    #+#             */
/*   Updated: 2025/06/05 15:07:37 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *get_path(char *cmd, t_data *data)
{
    char    **fpath;
    char    *t;
    char    *r;
    int     i;

    i = 0;
    if (cmd[0] == '/')
        return (ft_strdup(cmd));
    fpath = ft_split(check_env(data, "PATH="), ':');
    if (!fpath)
        return (NULL);
    t = ft_strjoin("/", cmd);
    while(fpath[i])
    {
        r = ft_strjoin(fpath[i], t);
		if (access(r, F_OK) == 0 && access(r, X_OK) == 0)
			return (ft_freemat((void **)fpath, (ft_matlen(fpath) - 1))
                , free(t), r);
		free(r);
		i++;
    }
    return (ft_freemat((void **)fpath, (ft_matlen(fpath) - 1)), free(t), NULL);
}

void    wait_status(t_data *data, pid_t last_pid)
{
    pid_t   ended;
    int status;

    status = 0;
    while (1)
    {
        ended = wait(&status);
        if (ended <= 0)
            break ;
        if (ended == last_pid)
        {
            if (WIFEXITED(status))
                data->status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                data->status = (128 + WTERMSIG(status));
        }
    }
    if (data->status == 130 || data->status == 131)
    {
        if (data->status == 131)
            ft_printf("Quit (core dumped)");
        ft_printf("\n");
    }
}

void    handle_fds(t_cmd *cmd, t_data *data)
{
    if (data->pipe)
        set_pipe(cmd, data);
    if (!do_open(cmd, data))
    {
        free_all(data, cmd);
        exit(1);
    }
    if (cmd->in_fd != 0)
        dup2(cmd->in_fd, STDIN_FILENO);
    if (cmd->ou_fd != 0)
        dup2(cmd->ou_fd, STDOUT_FILENO);
}

void    do_execve(t_cmd *cmd, t_data *data)
{
    char *path;
    char **args;

    path = ft_strdup(cmd->path);
    args = ft_matdup(cmd->args);
    if (cmd->in_fd != -1  && cmd->in_fd != 0 && !is_in_pipe(cmd->in_fd, data->pipe, data))
        close(cmd->in_fd);
    if (cmd->ou_fd != -1  && cmd->ou_fd != 0 && !is_in_pipe(cmd->ou_fd, data->pipe, data))
        close(cmd->ou_fd);
    free(cmd->path);
    free_data(data);
    execve(path, args, NULL);
    free(path);
    ft_freemat((void **)args, ft_matlen(args));
    exit(126);
}

pid_t    execute(t_cmd cmd, t_data *data)
{
    pid_t pid;

    // if (ft_strcmp(cmd_d.args[0], "exit") == 0 && !cmd_d.pipe_in && !cmd_d.pipe_out)
    //     handle_exit();
    pid = fork();
    if (pid == -1)
        ft_printf_fd(2, "fork error\n");
    if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        handle_fds(&cmd, data);
        if (!cmd.args[0])
        {
            free_all(data, &cmd);
            exit(0);
        }
        // if (!define_input(data, &cmd))
        // {
        cmd.path = get_path(cmd.args[0], data);
        if (!cmd.path)
        {
            write(2, cmd.args[0], ft_strlen(cmd.args[0]));
            write(2, ": command not found\n", 20);
            free_all(data, &cmd);
            exit(127);
        }
        do_execve(&cmd, data);
        // }
    }
    return (pid);
}
