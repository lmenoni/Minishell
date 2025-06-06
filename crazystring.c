/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crazystring.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 18:57:12 by igilani           #+#    #+#             */
/*   Updated: 2025/06/06 19:08:22 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_crazystring(char *s)
{
	char *prompt;
	char *temp = ft_strjoin(RED, s);
	char *temp2 = ft_strjoin(temp, RESET);
	prompt = ft_strjoin(temp2, YELLOW"> "RESET);
	free(temp);
	free(temp2);
	return (prompt);
}