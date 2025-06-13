/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 18:13:41 by igilani           #+#    #+#             */
/*   Updated: 2025/06/12 19:43:04 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*check_env(t_data *data, char *var)
{
	t_env	*temp;
	char	*name;
	int		i;

	i = 0;
	temp = data->env_data;
	while (temp)
	{
		i = 0;
		while (temp->e[i] != '\0' && temp->e[i] != '=')
			i++;
		name = ft_strndup(temp->e, i);
		if (ft_strcmp(name, var) == 0)
		{
			if (temp->e[i] == '\0')
				return (free(name), &temp->e[i]);
			else
				return (free(name), &temp->e[i + 1]);
		}
		temp = temp->next;
		free(name);
	}
	return (NULL);
}

void	update_env(t_data *data, char *var, char *str)
{
	t_env	*temp;
	char	*name;
	int		i;

	i = 0;
	temp = data->env_data;
	while (temp)
	{
		i = 0;
		while (temp->e[i] != '\0' && temp->e[i] != '=')
			i++;
		name = ft_strndup(temp->e, i);
		if (ft_strcmp(name, var) == 0)
		{
			free(temp->e);
			temp->e = ft_buffjoin(ft_strjoin(var, "="), str);
			return (free(name));
		}
		temp = temp->next;
		free(name);
	}
}

void	add_env(t_data *data, char *var)
{
	t_env	*new;
	t_env	*temp;

	new = new_env_node(ft_strdup(var));
	if (!data->env_data)
		data->env_data = new;
	else
	{
		temp = data->env_data;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
	}
}

void	delete_env(t_data *data, char *var)
{
	t_env	*temp;
	t_env	*prev;
	char	*var_name;

	temp = data->env_data;
	prev = NULL;
	while (temp)
	{
		var_name = get_var_name(temp->e);
		if (ft_strcmp(var_name, var) == 0)
		{
			if (prev)
				prev->next = temp->next;
			else
				data->env_data = temp->next;
			return (free(temp->e), free(temp), free(var_name));
		}
		prev = temp;
		temp = temp->next;
	}
	free(var_name);
}
