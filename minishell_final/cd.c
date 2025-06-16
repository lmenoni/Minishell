/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:25:13 by igilani           #+#    #+#             */
/*   Updated: 2025/06/16 16:23:32 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd_oldpwd(t_data *data, t_cmd *cmd, int *status)
{
	char	*path;

	path = NULL;
	if (data->old_path || check_env(data, "OLDPWD") != NULL)
	{
		if (data->old_path)
			path = data->old_path;
		else
			path = check_env(data, "OLDPWD");
		if (safe_chdir(path, status, NULL, NULL) == false)
			return ;
		ft_printf_fd(cmd->ou_fd, "%s\n", path);
		if (check_env(data, "OLDPWD"))
			update_env(data, "OLDPWD", data->current_path);
		free(data->old_path);
		data->old_path = data->current_path;
		data->current_path = getcwd(NULL, 0);
		update_env(data, "PWD", data->current_path);
	}
	else
	{
		ft_printf_fd(2, "minishell: cd: OLDPWD not set\n");
		*status = 1;
	}
}

void	cd_home(t_data *data, int *status)
{
	if (check_env(data, "HOME") != NULL)
	{
		if (safe_chdir(data->home_path, status, NULL, NULL) == false)
			return ;
		update_env(data, "OLDPWD", data->current_path);
		free(data->old_path);
		data->old_path = data->current_path;
		data->current_path = getcwd(NULL, 0);
		update_env(data, "PWD", data->current_path);
	}
	else
	{
		ft_printf_fd(2, "minishell: cd: HOME not set\n");
		*status = 1;
	}
}

void	cd_tilde(t_data *data, char *new_path, int *status)
{
	char	*joined_path;

	joined_path = ft_strjoin(data->home_path, new_path + 1);
	if (new_path[0] == '~' && new_path[1] == '\0')
	{
		if (safe_chdir(data->home_path, status, NULL, joined_path) == false)
			return ;
		update_env(data, "OLDPWD", data->current_path);
		free(data->old_path);
		data->old_path = data->current_path;
		data->current_path = getcwd(NULL, 0);
		update_env(data, "PWD", data->current_path);
	}
	else if (new_path[0] == '~' && new_path[1] != '\0')
	{
		if (safe_chdir(joined_path, status, NULL, joined_path) == false)
			return ;
		update_env(data, "OLDPWD", data->current_path);
		free(data->old_path);
		data->old_path = data->current_path;
		data->current_path = getcwd(NULL, 0);
		update_env(data, "PWD", data->current_path);
	}
}

void	cd_execution(t_data *data, char *new_path, int *status)
{
	if (safe_chdir(new_path, status, NULL, NULL) == false)
		return ;
	free(data->old_path);
	data->old_path = data->current_path;
	update_env(data, "OLDPWD", data->old_path);
	data->current_path = getcwd(NULL, 0);
	update_env(data, "PWD", data->current_path);
}

void	cd(t_data *data, t_cmd *cmd, char **args)
{
	char	*new_path;
	int		status;

	(void)cmd;
	status = 0;
	if (ft_matlen(args) <= 2)
	{
		new_path = args[1];
		if (new_path == NULL || ft_strcmp(new_path, "--") == 0)
			cd_home(data, &status);
		else if (new_path[0] == '\0')
			;
		else if (new_path[0] == '~')
			cd_tilde(data, new_path, &status);
		else if (ft_strcmp(new_path, "-") == 0)
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
