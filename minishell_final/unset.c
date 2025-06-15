/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 18:17:55 by igilani           #+#    #+#             */
/*   Updated: 2025/06/15 23:09:33 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int parse_unset(char **var)
{
	int i;

	i = 0;
	while (var[++i])
	{
		if (ft_strncmp(var[i], "-", 1) == 0)
		{
			ft_printf_fd(2, "minishell: unset: %c%c: invalid option\n", var[i][0], var[i][1]);
			return (1);
		}
	}
	return (0);
}

void	unset(t_data *data, char **var)
{
	int	i;

	i = 0;
	if (parse_unset(var))
	{
		data->status = 2;
		return ;
	}
	while (var[++i])
	{
		if (ft_strncmp(var[i], "OLDPWD", 6) == 0)
		{
			free(data->old_path);
			data->old_path = NULL;
		}
		delete_env(data, var[i]);
	}
}
