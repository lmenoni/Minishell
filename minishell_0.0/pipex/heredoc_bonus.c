/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 13:57:51 by igilani           #+#    #+#             */
/*   Updated: 2025/05/03 16:57:05 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void heredoc_read(char **argv, int *fd)
{
	char *line;
	
	close(fd[0]);
	while(1)
	{
		line = get_next_line(STDIN_FILENO);
		if (ft_strncmp(line, argv[3], ft_strlen(argv[3])) == 0)
		{
			free(line);
			exit(0);
		}
		ft_putstr_fd(line, fd[1]);
		free(line);
	}
}

void heredoc(char **argv)
{
	int fd[2];
	pid_t pid;

	if (pipe(fd) == -1)
		error_handle(1, 0);
	pid = fork();
	if (pid == -1)
		error_handle(2, 0);
	if (!pid)
		heredoc_read(argv, fd);
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		waitpid(pid, NULL, 0);
	}
}