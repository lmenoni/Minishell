/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 13:50:20 by igilani           #+#    #+#             */
/*   Updated: 2025/05/07 15:43:36 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_cmd_path(char *cmd, char **path_dirs)
{
	int		i;
	int		len;
	char	*full_path;

	if (ft_strchr(cmd, '/') != NULL)
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (cmd);
		return (NULL);
	}
	i = -1;
	while (path_dirs[++i])
	{
		len = ft_strlen(path_dirs[i]) + ft_strlen(cmd) + 2;
		full_path = malloc(len);
		if (!full_path)
			return (NULL);
		ft_strlcpy(full_path, path_dirs[i], len);
		ft_strlcat(full_path, "/", len);
		ft_strlcat(full_path, cmd, len);
		if (access(full_path, F_OK | X_OK) == 0)
			return (full_path);
		free(full_path);
	}
	return (NULL);
}

int open_file(char *file, int flags)
{
	int ret;

	ret = 0;
	if (flags == 0)
		ret = open(file, O_RDONLY, 0777);
	else if (flags == 1)
		ret = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	else if (flags == 2)
		ret = open(file, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (ret == -1)
		error_handle(5, 0);
	return (ret);
}

void do_pipe(char **cmd, char **env, int i)
{
	pid_t pid;
	int fd[2];

	if (pipe(fd) == -1)
		error_handle(1, 0);
	pid = fork();
	if (pid == -1)
		error_handle(2, 0);
	if (!pid)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		char **path = get_path(env, fd[1]);
		exec(cmd, env, path, i);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		waitpid(pid, NULL, 0);
	}
}

// < infile cmd1 | cmd2 > outfile <-- pipe
// < infile cmd1 | cmd2 | cmd3 > outfile <-- multiple pipes
// cmd1 << EOF | cmd2 > outfile <-- here_doc

int main(int argc, char **argv, char **env)
{
	int fd[2];
	int i;

	if (argc < 5)
		error_handle(0, 1);
	if (ft_strncmp(argv[1], "<<", 2) == 0)
	{
		if (argc < 6)
			error_handle(0, 1);
		i = 3;
		fd[1] = open_file(argv[argc - 1], 2);
		heredoc(argv);
	}
	else
	{
		i = 2;
		fd[0] = open_file(argv[1], 0);
		fd[1] = open_file(argv[argc - 1], 1);
		dup2(fd[0], STDIN_FILENO);
	}
	while (i < argc - 2)
		do_pipe(argv, env, i++);
	dup2(fd[1], STDOUT_FILENO);
	char **path = get_path(env, fd[0]);
	exec(argv, env, path, argc - 2);
}
