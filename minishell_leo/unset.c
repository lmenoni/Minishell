/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 18:17:55 by igilani           #+#    #+#             */
/*   Updated: 2025/05/20 14:51:26 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void unset(t_data *data, char *var)
{
	if (ft_strncmp(var, "OLDPWD", 6) == 0)
	{
		free(data->old_path);
		data->old_path = NULL;
	}
	delete_env(data, var);
}