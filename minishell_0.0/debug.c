/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:58:08 by igilani           #+#    #+#             */
/*   Updated: 2025/05/19 17:25:18 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_cd(t_data *data)
{
	printf("PWD: %s\n", data->current_path);
	printf("OLPWD: %s\n", data->old_path);
	printf("------------------\nEnv PWD: %s\n", check_env(data, "PWD="));
	printf("Env OLDPWD: %s\n", check_env(data, "OLDPWD="));
}
