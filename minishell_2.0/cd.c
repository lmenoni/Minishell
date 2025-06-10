/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:25:13 by igilani           #+#    #+#             */
/*   Updated: 2025/06/10 18:02:23 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
i comandi chdir e getcwd fanno delle chiamate di sistema e non sono condizionate dall'env
UNSET HOME funziona sempre ma non viene risettato se unsettato
una volta fatto UNSET di una variabile, non viene piu' risettata
OLDPWD funziona solo se e' settato
il comando pwd funziona sempre
se la HOME non e' settata e arriva una stringa vuota, ritorna errore con HOME non settata (casi unset $HOME, $PWD, cd (vuoto)), mentre con ~ funziona sempre
fare funzione safe_chdir per ridurre righe e che gestisce gli erorri anche

appena viene aperto minishell, OLDPWD non e' settato e cd - non funziona
bisogna controllare se OLDPWD esiste per poterlo aggiornare, quindi aggiungere check_env(data, "OLDPWD=") ma tenere sempre conto del =
*/

static bool safe_chdir(char *path, int *status, char *s1, char *s2)
{
	if (chdir(path) != 0)
	{
		
		ft_printf_fd(2, "minishell: cd: %s: no such file or directory\n", path);
		*status = 1;
		if (s1)
			free(s1);
		if (s2)
			free(s2);
		return (false);
	}
	return (true);
}

static void cd_oldpwd(t_data *data, t_cmd *cmd, int *status)
{
	char *path;
	
	path = NULL;
	if (data->old_path || check_env(data, "OLDPWD=") != NULL)
	{
		if (data->old_path)
			path = data->old_path;
		else
			path = check_env(data, "OLDPWD=");
		if (safe_chdir(path, status, NULL, NULL) == false)
			return ;
		ft_printf_fd(cmd->ou_fd, "%s\n", path);
		if (check_env(data, "OLDPWD="))
			update_env(data, "OLDPWD=", data->current_path);
		free(data->old_path);
		data->old_path = data->current_path;
		data->current_path = getcwd(NULL, 0);
		update_env(data, "PWD=", data->current_path);
	}
	else
	{
		ft_printf_fd(2, "minishell: cd: OLDPWD not set\n");
		*status = 1;
	}
}

void cd_home(t_data *data, int *status)
{
	char *added_equal;

	if (check_env(data, "HOME=") != NULL)
	{
		added_equal = ft_strjoin("=", data->current_path);
		if (safe_chdir(data->home_path, status, added_equal, NULL) == false)
			return ;
		update_env(data, "OLDPWD", added_equal);
		free(data->old_path);
		data->old_path = data->current_path;
		data->current_path = getcwd(NULL, 0);
		update_env(data, "PWD=", data->current_path);
	}
	else
	{
		ft_printf_fd(2, "minishell: cd: HOME not set\n");
		*status = 1;
	}
}

void cd_tilde(t_data *data, char *new_path, int *status)
{
	char *joined_path;
	char *added_equal;
	
	joined_path = ft_strjoin(data->home_path, new_path + 1);
	added_equal = ft_strjoin("=", data->current_path);
	if (new_path[0] == '~' && new_path[1] == '\0')
	{
		if (safe_chdir(data->home_path, status, added_equal, joined_path) == false)
			return ;
		update_env(data, "OLDPWD", added_equal);
		free(data->old_path);
		data->old_path = data->current_path;
		data->current_path = getcwd(NULL, 0);
		update_env(data, "PWD=", data->current_path);
	}
	else if (new_path[0] == '~' && new_path[1] != '\0')
	{
		if (safe_chdir(joined_path, status, added_equal, joined_path) == false)
			return ;
		update_env(data, "OLDPWD", added_equal);
		free(data->old_path);
		data->old_path = data->current_path;
		data->current_path = getcwd(NULL, 0);
		update_env(data, "PWD=", data->current_path);
	}
}

void cd_execution(t_data *data, char *new_path, int *status)
{
	char *added_equal;

	if (safe_chdir(new_path, status, NULL, NULL) == false)
			return ;
	free(data->old_path);
	data->old_path = data->current_path;
	added_equal = ft_strjoin("=", data->old_path);
	update_env(data, "OLDPWD", added_equal);
	free(added_equal);
	data->current_path = getcwd(NULL, 0);
	update_env(data, "PWD=", data->current_path);
}

void cd(t_data *data, t_cmd * cmd, char **args)
{
	char *new_path;
	int	status;
	(void)cmd;
	
	status = 0;
	if (ft_matlen(args) <= 2)
	{
		new_path = args[1];
		if (new_path == NULL)
			cd_home(data, &status);
		else if (new_path[0] == '\0')
			;
		else if (new_path[0] == '~')
			cd_tilde(data, new_path, &status);
		else if (new_path[0] == '-')
			cd_oldpwd(data, cmd, &status);
		else if (new_path != NULL)
			cd_execution(data, new_path, &status);
	}
	else
	{
		ft_printf_fd(2, "minishell: cd: too many arguments\n");
		status = 1;
	}
	data->status = status;
}
