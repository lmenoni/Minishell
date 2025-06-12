/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 18:13:41 by igilani           #+#    #+#             */
/*   Updated: 2025/06/12 17:44:50 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env   *new_env_node(char *s)
{
    t_env   *new;
    
    new = malloc(sizeof(t_env));
    new->e = s;
    new->next = NULL;
    return (new);
}

t_env   *init_env(char **env, t_data *data)
{
    t_env   *first;
    t_env   *curr;
    t_env   *new;
    int     i;

    first = NULL;
    i = 0;
    while (env[i])
    {
        curr = first;
        new = new_env_node(ft_strdup(env[i]));
        if (!first)
            first = new;
        else
        {
            while (curr->next)
                curr = curr->next;
            curr->next = new;
        }
        i++;
    }
    data->current_path = getcwd(NULL, 0);
    data->home_path = getenv("HOME");
    data->old_path = NULL;
    return (first);
}

// char *check_env(t_data *data, char *var)
// {
// 	t_env *temp;
// 	int i;
	
// 	i = 0;
// 	temp = data->env_data;
// 	while (temp)
// 	{
// 		i = 0;
// 		while (temp->e[i] != '\0')
// 		{
// 			if (i != 0 && temp->e[i - 1] == '=')
// 				break ;
// 			i++;
// 		}
// 		// ft_printf("env is: %s\n env name is: %d\n char is: %c\n len var is: %d\n", temp->e, i, temp->e[i], ft_strlen(var));
// 		// ft_printf("%d\n", ft_strnncmp(temp->e, var, i, ft_strlen(var)));
// 		if (ft_strnncmp(temp->e, var, i, ft_strlen(var)) == 0)
// 			return(&temp->e[i]);
// 		temp = temp->next;
// 	}
// 	return (NULL);
// }

char *check_env(t_data *data, char *var)
{
	t_env *temp;
	int i;
	
	i = 0;
	temp = data->env_data;
	while (temp)
	{
		i = 0;
		while (temp->e[i] != '\0' && temp->e[i] != '=')
			i++;
		if (ft_strncmp(temp->e, var, ft_strlen(var)) == 0)
			return(&temp->e[i + 1]);
		temp = temp->next;
	}
	return (NULL);
}

void update_env(t_data *data, char *var, char *str)
{
	t_env *temp;
	
	temp = data->env_data;
	while (temp)
	{
		if (ft_strncmp(temp->e, var, ft_strlen(var)) == 0)
		{
			free(temp->e);
			temp->e = ft_strjoin(var, str);
			break ;
		}
		temp = temp->next;
	}
}

void add_env(t_data *data, char *var)
{
	t_env *new;
	t_env *temp;

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

void delete_env(t_data *data, char *var)
{
	t_env *temp;
	t_env *prev;
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
			free(temp->e);
			free(temp);
			free(var_name);
			return ;
		}
		prev = temp;
		temp = temp->next;
	}
	free(var_name);
}

void env(t_data *data, t_cmd *cmd, char **input_array)
{
	t_env *curr;

	if (input_array[1] != NULL)
	{
		ft_printf_fd(2, "minishell: env: too many arguments\n");
		data->status = 127;
		return ;
	}
	curr = data->env_data;
	while (curr && curr->e)
	{
		if (!ft_strchr(curr->e, '='))
		{
			curr = curr->next;
			continue;
		}
		ft_printf_fd(cmd->ou_fd, "%s\n", curr->e);
		curr = curr->next;
	}
	data->status = 0;
}
