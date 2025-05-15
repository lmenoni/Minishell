/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 18:13:41 by igilani           #+#    #+#             */
/*   Updated: 2025/05/12 18:20:31 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void init_env(char **env, t_data *data)
{
	int i;

	i = 0;
	data->env_data = NULL;
	while (env[i])
	{
		add_env(data, env[i]);
		i++;
	}
}

void add_env(t_data *data, char *new_env)
{
	t_env *new_node;
	t_env *curr;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	new_node->e = ft_strdup(new_env);
	if (!new_node->e)
	{
		free(new_node);
		perror("strdup");
		exit(EXIT_FAILURE);
	}
	new_node->next = NULL;
	if (!data->env_data)
	{
		data->env_data = new_node;
		return;
	}
	curr = data->env_data;
	while (curr->next)
		curr = curr->next;
	curr->next = new_node;
}

void env(t_data *data)
{
	t_env *curr;

	curr = data->env_data;
	while (curr && curr->e)
	{
		printf("%s\n", curr->e);
		curr = curr->next;
	}
}