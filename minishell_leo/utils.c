/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:46:48 by lmenoni           #+#    #+#             */
/*   Updated: 2025/05/08 17:58:41 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_space(char c)
{
    return (c == ' ' || (c >= 9 && c <= 13));
}

void    skip_spaces(char *s, int *i)
{
    while (s[*i] != '\0' && is_space(s[*i]))
    {
        *i = *i + 1;
    }
}

void print_error(char *s)
{
    ft_putstr_fd(s, 2);
}