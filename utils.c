/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <lmenoni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:46:48 by lmenoni           #+#    #+#             */
/*   Updated: 2025/05/16 17:56:23 by lmenoni          ###   ########.fr       */
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

int parse_syntax_errors(t_token *token)
{
    while (token)
    {
        if (token->type == PIPE && !(token->next))
            return (ft_printf("minishell: syntax error near unexpected token `newline'\n"));
        if (token->type == PIPE && (!(token->prev) || token->next->type == PIPE))
            return (ft_printf("minishell: syntax error near unexpected token `|'\n"));
        if (token->type >= 2 && !(token->next))
            return (ft_printf("minishell: syntax error near unexpected token `newline'\n"));
        if (token->type >= 2 && (token->next->type >= 2 ||  token->next->type == PIPE))
            return (ft_printf("minishell: syntax error near unexpected token `>'\n"));
        token = token->next;
    }
    return (0);
}

void    reset_data(t_data *data)
{
    data->cmd_count = 0;
    data->cmd_arr = NULL;
    data->input = NULL;
    data->last_token = NULL;
    data->token = NULL;
}