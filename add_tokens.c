/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <lmenoni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:44:34 by lmenoni           #+#    #+#             */
/*   Updated: 2025/05/15 17:26:58 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int    argument_len(char *s)
{
    int i;
    char    quote;
    bool    in_quote;

    i = 0;
    quote = 0;
    if (s[i] != '\0' && (s[i] == '"' || s[i] == '\''))
    {
        in_quote = true;
        quote = s[i];
        i++;
    }
    else
        in_quote = false;
    while (in_quote || (!in_quote && s[i] != '\0' && s[i] != ' '
        && s[i] != '|' && s[i] != '<' && s[i] != '>'))
    {
        if ((!in_quote && (s[i] == '"' || s[i] == '\'')) || quote == s[i])
        {
            in_quote = !in_quote;
            quote = s[i];
        }
        i++;
    }
    return (i);
}

void    add_argument(t_data *data, char *s, int *idx)
{
    int i;
    int len;
    char *r;
    
    i = 0;
    len = argument_len(s);
    r = malloc((len + 1) * sizeof(char));
    //copertura malloc
    while (i < len)
    {
        r[i] = s[i];
        i++;
        *idx = *idx + 1;
    }
    r[i] = '\0';
    add_token(&data->token, r, ARGUMENT, data);
}

void    add_redirect(t_data *data, char *s, int *idx, char c)
{
    int i;
    int len;
    char type;
    char *r;
    
    i = 0;
    len = 0;
    type = s[i];
    while (s[i] != '\0' && s[i] == c && i < 2)
        i++;
    len = i;
    r = malloc((len + 1) * sizeof(char));
    //copertura malloc
    i = 0;
    while (i < len)
    {
        r[i] = s[i];
        i++;
        *idx = *idx + 1;
    }
    r[i] = '\0';
    add_token(&data->token, r, which_type(c, len), data);
}

void    add_pipe(t_data *data, char *s, int *idx)
{
    int i;
    char *r;
    
    i = 0;
    while (s[i] != '\0' && s[i] == '|')
        i++;
    // if (i != 1) piu pipe consecutivi
    r = malloc((2) * sizeof(char));
    //copertura malloc
    r[0] = '|';
    r[1] = '\0';
    *idx = *idx + 1;
    add_token(&data->token, r, PIPE, data);
}   
