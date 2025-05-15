/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:25:13 by igilani           #+#    #+#             */
/*   Updated: 2025/05/12 18:38:47 by igilani          ###   ########.fr       */
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

static int get_oldpwd_env(t_data *data)
{
    t_env *temp = data->env_data;

    while (temp)
    {
        if (ft_strncmp(temp->e, "OLDPWD=", 7) == 0)
            return (1);
        temp = temp->next;
    }
    return (0);
}

static void get_home_path(t_data *data)
{
    t_env *temp = data->env_data;

    while (temp && ft_strncmp(temp->e, "HOME=", 5) != 0)
        temp = temp->next;

    if (temp)
        data->home_path = temp->e + 5;
    else
        data->home_path = NULL;
}

void cd(t_data *data)
{
	char *new_path;

	new_path = data->input_array[1];
	get_home_path(data);
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
		if (get_oldpwd_env(data) == 0 && data->old_path == NULL)
		{
			print_error("bash: cd: OLDPWD not set\n");
			return ;
		}
		else if (chdir(data->old_path) != 0)
		{
			printf("%s\n", data->old_path);
			print_error("bash: cd: no such file or directory\n");
			return ;
		}
		ft_printf("%s\n", data->old_path);
		data->current_path = getcwd(NULL, 4096);
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
