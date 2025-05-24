/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:35:46 by igilani           #+#    #+#             */
/*   Updated: 2025/05/24 19:18:34 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
controlla se già esiste e nel caso aggiornarlo solo se non è vuoto, ma se c'è l'uguale va gestito come stringa nulla (es. a="")
ritorna erroe se il nome della variabile non inizia con una lettera o underscore
b==="a="a -> b="==a=a" arriva senza virgolette quindi bisogna controllare che dopo l'uguale tutto il resto va virgolettato come stringa
controllare come mi arriva l'argomento, se con o senza apici singoli, in caso con bisogna rimuoverli
*/

int parse_export(char **args)
{
	int i;
	i = 0;

	while (args[i])
	{
		if (args[i][0] == '=' || (!ft_isalpha(args[i][0]) && args[i][0] != '_'))
		{
			print_error("bash: export: not a valid identifier\n");
			return (1);
		}
		i++;
	}
	return (0);
}

void export (t_data *data)
{
	t_env *temp;
	char **args;
	args = data->input_array;
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
			if (i > 0 && temp->e[i] == '=')
				printf("declare -x %.*s=\"%s\"\n", i, temp->e, &temp->e[i + 1]);
			else
				printf("declare -x %s\n", temp->e);
			temp = temp->next;
		}
	}
	else
	{
		while (args[j])
		{
			if (parse_export(&args[j]) == 1)
			{
				j++;
				continue;
			}
			else
			{
				add_env(data, args[j]);
				j++;
			}
		}
	}
}
