/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:58:08 by igilani           #+#    #+#             */
/*   Updated: 2025/05/15 17:48:37 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_cd(t_data *data)
{
	printf("Current directory: %s\n", data->current_path);
	printf("OLPWD: %s\n", data->old_path);
	printf("Home directory: %s\n", data->home_path);
	printf("Env OLDPWD: %s\n", check_env(data, "OLDPWD="));
}
