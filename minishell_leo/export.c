/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:35:46 by igilani           #+#    #+#             */
/*   Updated: 2025/05/20 17:12:19 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
controlla se già esiste e nel caso aggiornarlo solo se non è vuoto, ma se c'è l'uguale va gestito come stringa nulla (es. a="")
*/

void parse_export(t_data *data, char *var)
{
	
}

void export (t_data *data, char **args)
{
	t_env *temp;
	int i;
	int j;

	i = 0;
	j = 1;
	temp = data->env_data;
	if (!args[1])
	{
		while (temp)
		{
			i = 0;
			while (temp->e[i] != '\0' && temp->e[i] != '=')
				i++;
			if (i > 0)
				printf("declare -x %.*s=\"%s\"\n", i, temp->e, &temp->e[i + 1]);
			else
				printf("declare -x %s\n", temp->e);
			temp = temp->next;
		}
	}
	else
	{
		while (args[j])
			parse_export(data, args[j++]);
	}
}
