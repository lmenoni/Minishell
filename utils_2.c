/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 12:57:42 by lmenoni           #+#    #+#             */
/*   Updated: 2025/05/30 12:57:44 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int tok_len(t_token *tok)
{
    int len;

    len = 0;
    while (tok)
    {
        tok = tok->next;
        len++;
    }
    return (len);
}

char    *get_unquoted(char *s)
{
    int i;
    int j;
    char    *r;

    i = 0;
    j = 0;
    while (s[i] != '\0' && s[i] != '"' && s[i] != '\'')
        i++;
    if (s[i] == '\0')
        return (s);
    r = malloc((len_wo_quotes(s) + 1) * sizeof(char));
    i = 0;
    while (s[i] != '\0')
    {
        if ((s[i] == '"' && is_quoted(s, &s[i]) != 1 ) || (s[i] == '\'' && is_quoted(s, &s[i]) != 2))
            i++;
        else
        {
            r[j] = s[i];
            i++;
            j++;
        }
    }
    r[j] = '\0';
    return (free(s), r);
}

int len_wo_quotes(char *s)
{
    int i;
    int len;

    i = 0;
    len = 0;
    while (s[i] != '\0')
    {
        if ((s[i] == '"' || s[i] == '\'') && !is_quoted(s, &s[i]))
            i++;
        else
        {
            i++;
            len++;
        }
    }
    return (len);
}

int parse_quotes(char *input)
{
    int i;

    i = 0;
    while (input[i] != '\0')
        i++;
    if (is_quoted(input, &input[i]) != 0)
        return (ft_printf_fd(2, "minishell: syntax error quotes left open\n"));
    return (0);
}