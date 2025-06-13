/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 14:43:34 by lmenoni           #+#    #+#             */
/*   Updated: 2025/06/13 14:43:36 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*check_path(char *cmd, t_data *data, t_cmd *s_cmd)
{
	struct stat statbuf;

	if (cmd[0] == '.' && cmd[1] == '\0')
		return (ft_printf_fd(2, "minishell: %s: filename argument required\n", cmd), NULL);
	if (access(cmd, F_OK) != 0)
		return (ft_printf_fd(2, "minishell: %s: No such file or directory\n", cmd), NULL);
	stat(cmd, &statbuf);
	if (S_ISDIR(statbuf.st_mode))
	{
		ft_printf_fd(2, "minishell: %s: Is a directory\n", cmd);
		free_exit(data, s_cmd, 126);
	}
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

char    *get_path(char *cmd, t_data *data, t_cmd *s_cmd)
{
    if (cmd[0] == '\0')
		return (ft_printf_fd(2, "%s: command not found\n", cmd), NULL);
	if (cmd[0] == '~')
		return (check_path(data->home_path, data, s_cmd));
	if (cmd[0] == '/' || (cmd[0] == '.'
		&& cmd[1] != '.') || !check_env(data, "PATH"))
		return (check_path(cmd, data, s_cmd));
	return (get_absolute(cmd, data));
}
