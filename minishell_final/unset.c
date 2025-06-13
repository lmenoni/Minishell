/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 18:17:55 by igilani           #+#    #+#             */
/*   Updated: 2025/06/12 19:31:31 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void unset(t_data *data, char **var)
{
	int i;

	i = 0;
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