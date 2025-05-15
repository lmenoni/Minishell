/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:44:34 by lmenoni           #+#    #+#             */
/*   Updated: 2025/05/05 16:11:54 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    add_argument(t_data *data, char *s, int *idx)
{
    int i;
    int j;
    char *r;
    
    i = 0;
    j = 0;
    while (s[i] != '\0' && s[i] != ' ' && s[i] != '<' && s[i] != '>' && s[i] != '|')
        i++;
    r = malloc((i + 1) * sizeof(char));
    //copertura malloc
    i = 0;
    while (s[i] != '\0' && s[i] != ' ' && s[i] != '<' && s[i] != '>' && s[i] != '|')
    {
        r[j] = s[i];
        i++;
        j++;
        *idx = *idx + 1;
    }
    r[j] = '\0';
    add_token(&data->token, r, ARGUMENT, data);
}

void    add_operator(t_data *data, char *s, int *idx)
{
    int i;
    char *r;
    
    i = 0;
    while (s[i] != '\0' && s[i] != ' ')
        i++;
    r = malloc((i + 1) * sizeof(char));
    //copertura malloc
    i = 0;
    while (s[i] != '\0' && s[i] != ' ')
    {
        r[i] = s[i];
        i++;
        *idx = *idx + 1;
    }
    r[i] = '\0';
    add_token(&data->token, r, OPERATOR, data);
}

void    add_redirect(t_data *data, char *s, int *idx)
{
    int i;
    // char type;
    char *r;
    
    i = 0;
    // type = s[i];
    while (s[i] != '\0' && (s[i] == '<' || s[i] == '>'))
    {
        //if (s[i] != type)
        //    errore in input (redirect diversi consecutivi) ;
        i++;
    }
    r = malloc((i + 1) * sizeof(char));
    //copertura malloc
    i = 0;
    while (s[i] != '\0' && (s[i] == '<' || s[i] == '>'))
    {
        r[i] = s[i];
        i++;
        *idx = *idx + 1;
    }
    r[i] = '\0';
    add_token(&data->token, r, REDIRECT, data);
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

void    add_string(t_data *data, char *s, int *idx)
{
    int i;
    int j;
    char *r;
    
    i = 0;
    j = 0;
    while (s[i] != '\0' && s[i] != '"')
        i++;
    r = malloc((i + 1) * sizeof(char));
    //copertura malloc
    i = 0;
    while (s[i] != '\0' && s[i] != '"')
    {
        r[j] = s[i];
        i++;
        j++;
        *idx = *idx + 1;
    }
    r[j] = '\0';
    add_token(&data->token, r, STRING, data);
}