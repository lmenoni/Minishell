/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 18:39:51 by igilani           #+#    #+#             */
/*   Updated: 2025/06/06 16:53:51 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void pwd()
{
	char *current_path;

	current_path = getcwd(NULL, 0);
	if (current_path == NULL)
	{
		perror("getcwd");
		exit(1);
	}
	ft_printf("%s\n", current_path);
	free(current_path);
	//exit(0);
}