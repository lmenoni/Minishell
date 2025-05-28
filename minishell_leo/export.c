/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:35:46 by igilani           #+#    #+#             */
/*   Updated: 2025/05/28 13:54:58 by igilani          ###   ########.fr       */
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
	int i;

	i = 1;
	if (var[0] == '=' || (!ft_isalpha(var[0]) && var[0] != '_'))
	{
		print_error("bash: export: not a valid identifier\n"); //sistemare con printf per stamapre anche var
		return (1);
	}
	while (var[i] && var[i] != '=')
	{
		if (var[i] == '+' && var[i + 1] == '=')
			return (2);
		else if (!ft_isalnum(var[i]) && var[i] != '_')
		{
			print_error("bash: export: not a valid identifier\n");
			return (1);
		}
		i++;
	}
	return (0);
}

char *get_var_name(char *var)
{
	int i;

	i = 0;
	while (var[i] && (var[i] != '=' && var[i] != '+'))
		i++;
	return (ft_substr(var, 0, i));
}

void add_export(t_data *data, char **args)
{
	int i;
	int parse_result;
	char *var_name;
	char *temp;

	i = 1;
	while (args[i])
	{
		parse_result = parse_export(args[i]);
		var_name = get_var_name(args[i]);
		if (parse_result == 0)
		{
			if (check_env(data, args[i]) != NULL)
				update_env(data, var_name, args[i] + ft_strlen(var_name));
			else
				add_env(data, args[i]);
		}
		else if (parse_result == 2)
		{
			if (check_env(data, args[i]) != NULL)
			{
				temp = ft_strjoin("=", ft_strjoin(check_env(data, args[i]), args[i] + ft_strlen(var_name) + 2));
				update_env(data, var_name, temp);
				
			}
			else
			{
				temp = ft_strjoin(var_name, args[i] + ft_strlen(var_name) + 1);
				add_env(data, temp);
			}
			free(temp);
		}
		free(var_name);
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
				printf("declare -x %.*s=\"%s\"\n", i, temp->e, &temp->e[i + 1]);//da cambiare con write
			else
				printf("declare -x %s\n", temp->e);
			temp = temp->next;
		}
	}
	else
		add_export(data, args);
}
