/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 14:10:21 by lmenoni           #+#    #+#             */
/*   Updated: 2025/06/03 14:10:23 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    while_var(char *s, int *i)
{
    *i = *i + 1;
    while(s[*i] != '\0' && s[*i] != '$' && s[*i] != '"' && s[*i] != '\'')
        *i = *i + 1;
}

void    while_string(char *s, int *i)
{
    while (s[*i] != '\0')
    {
        if (s[*i] == '$' && s[*i + 1] != '\0' && !is_space(s[*i + 1]) && is_quoted(s, &s[*i]) == 0)
            break ;
        *i = *i + 1;
    }
}

int count_pieces(char *s)
{
    int i;
    int r;

    i = 0;
    r = 0;
    while (s[i] != '\0')
    {
        if (s[i] != '$')
        {
            while_string(s, &i);
            r++;
        }
        else
        {
            if (s[i + 1] == '"' || s[i + 1] == '\'' || s[i + 1] == '$')
                i++;
            else
            {
                while_var(s, &i);
                r++;
            }
        }
    }
    return (r);
}

char    last_char(char *s)
{
    if (*s == '\0')
        return ('\0');
    while (*(s + 1) != '\0')
        s++;
    return (*s);
}

bool    is_dollar_quoted(char *s)
{
    int i;

    i = 0;
    while (s[i] != '\0')
    {
        if (s[i] == '$' && s[i + 1] != '\0' && !is_space(s[i + 1]) && is_quoted(s, &s[i]) == 0)
            return (false);
        i++;
    }
    return (true);
}
