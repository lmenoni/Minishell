/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 18:17:55 by igilani           #+#    #+#             */
/*   Updated: 2025/05/12 18:37:58 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void unset(t_data *data, char *var)
{
	t_env *curr;
	t_env *prev;

	curr = data->env_data;
	prev = NULL;
	if (ft_strncmp(var, "OLDPWD", 6) == 0)
	{
		free(data->old_path);
		data->old_path = NULL;
	}
	while (curr)
	{
		if (ft_strncmp(curr->e, var, ft_strlen(var)) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				data->env_data = curr->next;
			free(curr->e);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}