/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:35:46 by igilani           #+#    #+#             */
/*   Updated: 2025/05/25 19:57:46 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
controlla se già esiste e nel caso aggiornarlo solo se non è vuoto, ma se c'è l'uguale va gestito come stringa nulla (es. a="")
ritorna erroe se il nome della variabile non inizia con una lettera o underscore
b==="a="a -> b="==a=a" arriva senza virgolette quindi bisogna controllare che dopo l'uguale tutto il resto va virgolettato come stringa
controllare come mi arriva l'argomento, se con o senza apici singoli, in caso con bisogna rimuoverli
se la variabile esiste già, bisogna aggiornarla con il nuovo valore, ma se si passa il nome della varibile senza valore bisogna non aggiornarla, ma se viene inserito un uguale bisogna aggiornarla con una stringa vuota
*/

int parse_export(char *var)
{
	if (var[0] == '=' || (!ft_isalpha(var[0]) && var[0] != '_'))
	{
		print_error("bash: export: not a valid identifier\n"); //sistemare con printf per stamapre anche var
		return (1);
	}
	return (0);
}

char *get_var_name(char *var)
{
	int i;

	i = 0;
	while (var[i] && var[i] != '=')
		i++;
	return (ft_substr(var, 0, i));
}

void add_export(t_data *data, char **args)
{
	int i;

	i = 1;
	while (args[i])
	{
		if (parse_export(args[i]) == 0)
		{
			if (check_env(data, args[i]) != NULL)
			{
				printf("%s\n", check_env(data, args[i]));//da rivedere perchè quando esiste una variabile con inizio uguale non viene creata
				update_env(data, get_var_name(args[i]), args[i] + ft_strlen(get_var_name(args[i])));
			}
			else
				add_env(data, args[i]);
		}
		i++;
	}
}

void export (t_data *data, char **args)
{
	t_env *temp;
	int i;

	i = 0;
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
		add_export(data, args);
}
