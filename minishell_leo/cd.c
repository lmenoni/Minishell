/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:25:13 by igilani           #+#    #+#             */
/*   Updated: 2025/05/19 18:28:33 by igilani          ###   ########.fr       */
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
se la HOME non e' settata e arriva una stringa vuota, ritorna errore con HOME non settata (casi unset $HOME, $PWD, cd (vuoto)), mentre con ~ funziona sempre
fare funzione safe_chdir per ridurre righe e che gestisce gli erorri anche
*/

static void safe_chdir(char *path)
{
	if (chdir(path) != 0)
	{
		print_error("bash: cd: no such file or directory\n");
		return ;
	}
}

static int cd_parse(t_data *data)
{
	int i;

	i = 0;
	while (data->input_array[++i])
	{
		if (i >= 2)
			return (0);
	}
	return (1);
}

static void cd_oldpwd(t_data *data)
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
			update_env(data, "OLDPWD=", data->current_path);
		ft_printf("%s\n", path);
		free(data->old_path);
		data->old_path = data->current_path;
		data->current_path = getcwd(NULL, 0);
		update_env(data, "PWD=", data->current_path);
	}
	else
		print_error("bash: cd: OLDPWD not set\n");
}

void cd_home(t_data *data)
{
	if (check_env(data, "HOME=") != NULL)
	{
		update_env(data, "OLDPWD=", data->current_path);
		free(data->old_path);
		data->old_path = data->current_path;
		safe_chdir(data->home_path);
		data->current_path = getcwd(NULL, 0);
		update_env(data, "PWD=", data->current_path);
	}
	else
	{
		print_error("bash: cd: HOME not set\n");
		return ;
	}
}

void cd_tilde(t_data *data, char *new_path)
{
	char *joined_path;
	
	joined_path = ft_strjoin(data->home_path, new_path + 1);
	if (new_path[0] == '~' && new_path[1] == '\0')
	{
		update_env(data, "OLDPWD=", data->current_path);
		free(data->old_path);
		data->old_path = data->current_path;
		safe_chdir(data->home_path);
		data->current_path = getcwd(NULL, 0);
		update_env(data, "PWD=", data->current_path);
	}
	else if (new_path[0] == '~' && new_path[1] != '\0')
	{
		
		update_env(data, "OLDPWD=", data->current_path);
		free(data->old_path);
		data->old_path = data->current_path;
		safe_chdir(joined_path);
		data->current_path = getcwd(NULL, 0);
		update_env(data, "PWD=", data->current_path);
	}
	free(joined_path);
}

void cd(t_data *data)
{
	char *new_path;

	if (cd_parse(data) == 0)
	{
		print_error("bash: cd: too many arguments\n");
		return ;
	}
	new_path = data->input_array[1];
	if (new_path == NULL)
		cd_home(data);
	else if (new_path[0] == '~')// if (new_path[1] != '\0') strjoin(data->home_path, new_path + 1)
		cd_tilde(data, new_path);
	else if (new_path[0] == '-')
		cd_oldpwd(data);
	else if (new_path != NULL)
	{
		free(data->old_path);
		data->old_path = data->current_path;
		update_env(data, "OLDPWD=", data->old_path);
		safe_chdir(new_path);
		data->current_path = getcwd(NULL, 0);
		update_env(data, "PWD=", data->current_path);
	}
}
