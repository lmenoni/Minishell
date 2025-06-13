/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 14:22:42 by lmenoni           #+#    #+#             */
/*   Updated: 2025/06/13 14:22:44 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool    long_overflow(char *s)
{
    int			sign;
	long long	r;
	int		    i;

	sign = 1;
	r = 0;
	i = 0;
    skip_spaces(s, &i);
	if (s[i] == '-' || s[i] == '+')
	{
		if (s[i] == '-')
			sign = -1;
		i++;
	}
	while (s[i] >= '0' && s[i] <= '9')
	{
		if ((sign == 1 && r > (LLONG_MAX - (s[i] - '0')) / 10)
			|| (sign == -1 && -r < (LLONG_MIN + (s[i] - '0')) / 10))
			return (true);
		r = (r * 10) + (s[i] - '0');
		i++;
	}
	return (false);
}

void    close_if(t_cmd *cmd, t_data *data)
{
    if (cmd->in_fd != -1  && cmd->in_fd != data->st_in
		&& !is_in_pipe(cmd->in_fd, data->pipe, data))
		close(cmd->in_fd);
	if (cmd->ou_fd != -1  && cmd->ou_fd != data->st_out
		&& !is_in_pipe(cmd->ou_fd, data->pipe, data))
		close(cmd->ou_fd);
}

void	free_exit(t_data *data, t_cmd *cmd, int status)
{
	free_all(data, cmd);
	exit(status);
}

char    **copy_env(t_env *env)
{
    int len;
    char **r;
    t_env *t;
    int     i;

    len = 0;
    i = 0;
    t = env;
    while (t)
    {
        len++;
        t = t->next;
    }
    r = malloc((len + 1) * sizeof(char *));
    t = env;
    while (i < len)
    {
        r[i] = ft_strdup(t->e);
        i++;
        t = t->next;
    }
    r[i] = NULL;
    return (r);
}
