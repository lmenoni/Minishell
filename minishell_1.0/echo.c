/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:43:55 by igilani           #+#    #+#             */
/*   Updated: 2025/06/06 19:32:06 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int check_echo_flag(char **input_array)
{
	int i;
	int j;
	
	i = 0;
	j = 1;

	while (input_array[j])
	{
		i = 0;
		if (input_array[j][0] == '-')
			i++;
		else
			return (j - 1);
		while (input_array[j][i] != '\0')
		{
			if (input_array[j][i] == 'n')
				i++;
			else
				return (j - 1);
		}
		j++;
	}
	return (j - 1);
}

void echo(char **args)
{
	int i;
	int flag;
	flag = check_echo_flag(args);

	if (flag > 0)
		i = flag + 1;
	else
		i = 1;
	if (args[i] == NULL)
	{
		ft_printf("\n");
		//free_all(data);
		exit(0);
	}
	while (args[i])
	{
		ft_printf("%s", args[i]);
		i++;
		if (args[i])
			ft_printf(" ");
	}
	if (!flag)
		ft_printf("\n");
	exit(0);
}