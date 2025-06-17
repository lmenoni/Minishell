/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 18:39:51 by igilani           #+#    #+#             */
/*   Updated: 2025/06/10 17:53:25 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd(t_data *data, t_cmd *cmd)
{
	ft_printf_fd(cmd->ou_fd, "%s\n", data->current_path);
	data->status = 0;
}
