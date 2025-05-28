/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:35:46 by igilani           #+#    #+#             */
/*   Updated: 2025/05/28 18:28:19 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
controlla se già esiste e nel caso aggiornarlo solo se non è vuoto, ma se c'è l'uguale va gestito come stringa nulla (es. a="")
ritorna erroe se il nome della variabile non inizia con una lettera o underscore
b==="a="a -> b="==a=a" arriva senza virgolette quindi bisogna controllare che dopo l'uguale tutto il resto va virgolettato come stringa
controllare come mi arriva l'argomento, se con o senza apici singoli, in caso con bisogna rimuoverli
se la variabile esiste già, bisogna aggiornarla con il nuovo valore, ma se si passa il nome della varibile senza valore bisogna non aggiornarla, ma se viene inserito un uguale bisogna aggiornarla con una stringa vuota

if (i > 0 && env->e[i] == '=')
	printf("declare -x %.*s=\"%s\"\n", i, env->e, &env->e[i + 1]);//da cambiare con write
else
	printf("declare -x %s\n", env->e);
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

void export_plus(t_data *data, char *args, char *var_name)
{
	char *temp;
	char *replace;
	
	if (check_env(data, var_name) != NULL)
	{
		temp = ft_strjoin(check_env(data, var_name), args + ft_strlen(var_name) + 2);
		replace = ft_strjoin("=", temp);
		update_env(data, var_name, replace);
		free(temp);
	}
	else
	{
		replace = ft_strjoin(var_name, args + ft_strlen(var_name) + 1);
		add_env(data, replace);
	}
	free(replace);
}

void export_add(t_data *data, char *args, char *var_name)
{
	if (check_env(data, var_name) != NULL && ft_strchr(args, '=') != NULL)
		update_env(data, var_name, args + ft_strlen(var_name));
	else if (check_env(data, var_name) == NULL)
		add_env(data, args);
	else
		return ;
}

void export_case(t_data *data, char **args)
{
	int i;
	int parse_result;
	char *var_name;

	i = 1;
	while (args[i])
	{
		parse_result = parse_export(args[i]);
		var_name = get_var_name(args[i]);
		if (parse_result == 0)
			export_add(data, args[i], var_name);
		else if (parse_result == 2)
			export_plus(data, args[i], var_name);
		free(var_name);
		i++;
	}
}

void print_export(t_env *env, int i)
{
	if (i > 0 && env->e[i] == '=')
	{
	    char *var_name = get_var_name(env->e);
	    char *part1 = ft_strjoin("declare -x ", var_name);
	    char *part2 = ft_strjoin(part1, "=\"");
	    char *part3 = ft_strjoin(part2, &env->e[i + 1]);
	    char *full_str = ft_strjoin(part3, "\"\n");
	    write(STDOUT_FILENO, full_str, ft_strlen(full_str));
	    free(var_name);
	    free(part1);
	    free(part2);
	    free(part3);
	    free(full_str);
	}
	else
	{
	    char *part1 = ft_strjoin("declare -x ", env->e);
	    char *full_str = ft_strjoin(part1, "\n");
	    write(STDOUT_FILENO, full_str, ft_strlen(full_str));
	    free(part1);
	    free(full_str);
	}
}

void export(t_data *data, char **args)
{
	t_env *env;
	int i;

	i = 0;
	env = data->env_data;
	if (!args[1])
	{
		while (env)
		{
			i = 0;
			while (env->e[i] != '\0' && env->e[i] != '=')
				i++;
			print_export(env, i);
			env = env->next;
		}
	}
	else
		export_case(data, args);
}
