/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:25:13 by igilani           #+#    #+#             */
/*   Updated: 2025/05/15 18:51:33 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
i comandi chdir e getcwd fanno delle chiamate di sistema e non sono condizionate dall'env
UNSET HOME funziona sempre ma non viene risettato se unsettato
una volta fatto UNSET di una variabile, non viene piu' risettata
OLDPWD funziona solo se e' settato
il comando pwd funziona sempre
OLDPWD inzia a NULL e viene comparto con il OLDPWD di env, viene settato il nosto OLDPWD ad ogni chiamata di cd e messo a NULL ogni volta che in unset viene fatto l'unset di OLDPWD
*/

char *check_env(t_data *data, char *var)
{
	t_env *temp;
	
	temp = data->env_data;
	while (temp)
	{
		if (ft_strncmp(temp->e, var, ft_strlen(var)) == 0)
			return(&temp->e[ft_strlen(var) - 1]);
		temp = temp->next;
	}
	return (NULL);
}

// static int get_oldpwd_env(t_data *data)
// {
//     t_env *temp = data->env_data;

//     while (temp)
//     {
//         if (ft_strncmp(temp->e, "OLDPWD=", 7) == 0)
//             return (1);
//         temp = temp->next;
//     }
//     return (0);
// }

// void get_home_path(t_data *data)
// {
//     t_env *temp = data->env_data;

//     while (temp && ft_strncmp(temp->e, "HOME=", 5) != 0)
//         temp = temp->next;

//     if (temp)
//         data->home_path = temp->e + 5;
//     else
//         data->home_path = NULL;
// }

void    update_oldpath(t_data *data) //da rendere globale con attenzione a passare due stringh NULL terminate per controllare
{
	t_env *temp;

	temp = data->env_data;
	while (temp)
	{
		if (ft_strncmp(temp->e, "OLDPWD=", 7) == 0)
		{
			free(temp->e);
			temp->e = ft_strjoin("OLDPWD=", data->current_path);
			break ;
		}
		temp = temp->next;
	}
}

void cd_oldpwd(t_data *data)
{
	char *path;
	
	path = NULL;
	if (data->old_path || check_env(data, "OLDPWD=") != NULL)
	{
		if (data->old_path)
			path = data->old_path;
		else
			path = check_env(data, "OLDPWD=");
		chdir(path);
		if (check_env(data, "OLDPWD="))
			update_oldpath(data);
		ft_printf("%s\n", path);
		data->old_path = data->current_path;
		data->current_path = getcwd(NULL, 4096);
	}
	else 
		print_error("bash: cd: OLDPWD not set\n");
}

void cd(t_data *data)
{
	char *new_path;

	new_path = data->input_array[1];
	// get_home_path(data);
	if (new_path == NULL || (new_path[0] == '~' && new_path[1] == '\0'))
	{
		data->old_path = getcwd(NULL, 4096);
		if (chdir(data->home_path) != 0)
		{
			print_error("bash: cd: no such file or directory\n");
			return ;
		}
		data->current_path = getcwd(NULL, 4096);
	}
	else if (new_path[0] == '~' && new_path[1] != '\0')
	{
		data->old_path = getcwd(NULL, 4096);
		if (chdir(ft_strjoin(data->home_path, new_path + 1)) != 0)
		{
			print_error("bash: cd: no such file or directory\n");
			return ;
		}
		data->current_path = getcwd(NULL, 4096);
	}
	else if (new_path[0] == '-')
	{
		cd_oldpwd(data);
	}
	else if (new_path != NULL)
	{
		data->old_path = getcwd(NULL, 4096);
		if (chdir(new_path) != 0)
		{
			print_error("bash: cd: no such file or directory\n");
			return ;
		}
		data->current_path = getcwd(NULL, 4096);
	}
}
