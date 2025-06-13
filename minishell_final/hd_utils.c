/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 14:07:12 by lmenoni           #+#    #+#             */
/*   Updated: 2025/06/13 14:07:16 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	here_sigint(int sig)
{
	char	c;

	c = '\n';
	(void)sig;
	g_last_signal = 1;
	ioctl(0, TIOCSTI, &c);
	rl_on_new_line();
}

bool	check_lstsig_here(char *line, char *limiter, char *r, t_data *data)
{
	if (g_last_signal == 1)
	{
		if (r)
			free(r);
		free(line);
		free(limiter);
		data->status = 130;
		return (true);
	}
	return (false);
}

bool	reset_signal(void)
{
	if (g_last_signal)
	{
		g_last_signal = 0;
		signal(SIGINT, handle_sigint);
		return (false);
	}
	signal(SIGINT, handle_sigint);
	return (true);
}

char	*ft_crazystring(char *s)
{
	char	*prompt;
	char	*temp;
	char	*temp2;

	temp = ft_strjoin(MAGENTA, s);
	temp2 = ft_strjoin(temp, RESET);
	prompt = ft_strjoin(temp2, YELLOW"> "RESET);
	free(temp);
	free(temp2);
	return (prompt);
}
