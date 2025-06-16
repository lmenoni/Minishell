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

void	wait_status(t_data *data, pid_t last_pid)
{
	pid_t	ended;
	int		status;

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

void	do_execve(t_cmd *cmd, t_data *data)
{
	char	*path;
	char	**args;
	char	**env;

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

void	children(t_cmd *cmd, t_data *data)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (!cmd->args[0])
		free_exit(data, cmd, 0);
	if (define_input(data, cmd))
		free_exit(data, cmd, data->status);
	cmd->path = get_path(cmd->args[0], data, cmd);
	if (!cmd->path)
		free_exit(data, cmd, 127);
	do_execve(cmd, data);
}

pid_t	execute(t_cmd cmd, t_data *data)
{
	pid_t	pid;

	cmd.in_fd = data->st_in;
	cmd.ou_fd = data->st_out;
	if (!handle_fds(&cmd, data))
		return (close_if(&cmd, data), 0);
	if (!cmd.args[0])
		return (close_if(&cmd, data), 0);
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
