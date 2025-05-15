/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 18:52:26 by igilani           #+#    #+#             */
/*   Updated: 2025/05/03 16:49:37 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_handle(int error, int exit_type)
{
	if (error == 0)
		ft_putstr_fd("Usage: ./pipex infile cmd1 cmd2 outfile\n", 2); //da modificare per il bonus
	else if (error == 1)
		perror("pipex: pipe");
	else if (error == 2)
		perror("pipex: fork");
	else if (error == 3)
		ft_putstr_fd("zsh: command not found\n", 2);
	else if (error == 4)
		perror("pipex: execve");
	else if (error == 5 || error > 7)
		perror("pipex");
	else if (error == 7)
		ft_putstr_fd("pipex: PATH not set\n", 2);
	else if (error == 8)
		perror("pipex");
	exit(exit_type);
}

void	ft_free(char **mat)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

static void	ft_cmd_not_found(char *cmd, char **cmd_array, char **path)
{
	ft_putstr_fd("zsh: command not found: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd("\n", 2);
	ft_free(cmd_array);
	ft_free(path);
	close(0);
	close(1);
	exit(127);
}

static char	**parse_cmd(char *arg, char **path)
{
	char	**cmd;

	if (!arg || arg[0] == '\0')
	{
		ft_free(path);
		close(0);
		close(1);
		error_handle(3, 127);
	}
	cmd = ft_split(arg, ' ');
	if (!cmd || !cmd[0])
	{
		if (cmd)
			ft_free(cmd);
		ft_free(path);
		close(0);
		close(1);
		error_handle(3, 1);
	}
	return (cmd);
}

void	exec(char **argv, char **env, char **path, int cmd_index)
{
	char	*cmd_path;
	char	**cmd;

	cmd = parse_cmd(argv[cmd_index], path);
	cmd_path = find_cmd_path(cmd[0], path);
	if (!cmd_path)
		ft_cmd_not_found(cmd[0], cmd, path);
	if (execve(cmd_path, cmd, env) == -1)
	{
		ft_free(cmd);
		free(cmd_path);
		ft_free(path);
		close(0);
		close(1);
		error_handle(4, 0);
	}
}
