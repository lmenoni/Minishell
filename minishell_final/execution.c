/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 15:07:32 by lmenoni           #+#    #+#             */
/*   Updated: 2025/06/12 19:34:51 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*check_path(char *cmd)
{
	struct stat statbuf;

	if (cmd[0] == '.' && cmd[1] == '\0')
		return (ft_printf_fd(2, "minishell: %s: filename argument required\n", cmd), NULL);
	if (access(cmd, F_OK) != 0)
		return (ft_printf_fd(2, "minishell: %s: No such file or directory\n", cmd), NULL);
	stat(cmd, &statbuf);
	if (S_ISDIR(statbuf.st_mode))
		return (ft_printf_fd(2, "minishell: %s: Is a directory\n", cmd), NULL);
	if (access(cmd, X_OK) != 0)
		return (ft_printf_fd(2, "minishell: %s: Permission denied\n", cmd), NULL);
	return (ft_strdup(cmd));
}

char	*get_absolute(char *cmd, t_data *data)
{
	char    **fpath;
	char    *t;
	char    *r;
	int     i;

	i = 0;
	if (cmd[0] == '.' && cmd[1] == '.')
		return (ft_printf_fd(2, "%s: command not found\n", cmd)
			, NULL);
	fpath = ft_split(check_env(data, "PATH"), ':');
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
	ft_printf_fd(2, "%s: command not found\n", cmd);
	return (ft_freemat((void **)fpath
		, (ft_matlen(fpath) - 1)), free(t), NULL);
}

char    *get_path(char *cmd, t_data *data)
{
    if (cmd[0] == '\0')
		return (ft_printf_fd(2, "%s: command not found\n", cmd), NULL);
	if (cmd[0] == '~')
		return (check_path(data->home_path));
	if (cmd[0] == '/' || (cmd[0] == '.'
		&& cmd[1] != '.') || !check_env(data, "PATH"))
		return (check_path(cmd));
	return (get_absolute(cmd, data));
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

void    do_execve(t_cmd *cmd, t_data *data)
{
	char *path;
	char **args;
	char    **env;

	path = ft_strdup(cmd->path);
	args = ft_matdup(cmd->args);
	env = copy_env(data->env_data);
	close_if(cmd, data);
	free(cmd->path);
	free_data(data);
	execve(path, args, env);
	free(path);
	ft_freemat((void **)args, ft_matlen(args));
	free(env);
	exit(126);
}

void	free_exit(t_data *data, t_cmd *cmd, int status)
{
	free_all(data, cmd);
	exit(status);
}

void    children(t_cmd  *cmd, t_data *data)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (define_input(data, cmd))
		free_exit(data, cmd, data->status);
	if (!cmd->args[0])
		free_exit(data, cmd, 0);
	cmd->path = get_path(cmd->args[0], data);
	if (!cmd->path)
		free_exit(data, cmd, 127);
	do_execve(cmd, data);
}

bool	handle_fds(t_cmd *cmd, t_data *data)
{
	if (data->pipe)
		set_pipe(cmd, data);
	if (!do_open(cmd, data))
    {
        data->status = 1;
		close_if(cmd, data);
		return (false);
    }
	dup2(cmd->in_fd, STDIN_FILENO);
	dup2(cmd->ou_fd, STDOUT_FILENO);
	return (true);
}

pid_t    execute(t_cmd cmd, t_data *data)
{
	pid_t pid;

	cmd.in_fd = data->st_in;
	cmd.ou_fd = data->st_out;
	if (!handle_fds(&cmd, data))
		return (0);
	if (!data->pipe && define_input(data, &cmd))
		return (0);
	pid = fork();
	if (pid == -1)
		ft_printf_fd(2, "fork error\n");
	if (pid == 0)
		children(&cmd, data);
	close_if(&cmd, data);
	return (pid);
}
