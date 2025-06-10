/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 18:39:51 by igilani           #+#    #+#             */
/*   Updated: 2025/06/09 19:14:18 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void pwd(t_data *data, t_cmd *cmd)
{
	ft_printf("%s\n", data->current_path);
	free_all(data, cmd);
	exit(0);
}