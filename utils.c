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
            return (ft_printf_fd(2, "minishell: syntax error near unexpected token `newline'\n"));
        if (token->type == PIPE && (!(token->prev) || token->next->type == PIPE))
            return (ft_printf_fd(2, "minishell: syntax error near unexpected token `|'\n"));
        if (token->type >= 3 && !(token->next))
            return (ft_printf_fd(2, "minishell: syntax error near unexpected token `newline'\n"));
        if (token->type >= 3 && (token->next->type >= 3 ||  token->next->type == PIPE))
            return (ft_printf_fd(2, "minishell: syntax error near unexpected token `>'\n"));
        token = token->next;
    }
    return (0);
}

int	is_quoted(char *s, char *to_check)
{
	bool	in_quote;
	char	quote;

	quote = 0;
	in_quote = false;
	while (s != to_check)
	{
		if ((*s == '"' || *s == '\'') && (quote == *s || quote == 0))
		{
			in_quote = !in_quote;
			quote = *s;
		}
		if (!in_quote)
			quote = 0;
		s++;
	}
	if (*to_check == quote && in_quote)
		return (0);
	if (in_quote && quote == '\'')
		return (1);
	if (in_quote && quote == '"')
		return (2);
	return (0);
}

void    reset_data(t_data *data)
{
    free(data->input);
    free_token(data->token);
    free_cmd_array(data);
    data->cmd_count = 0;
    data->cmd_name = 0;
    data->cmd_arr = NULL;
    data->input = NULL;
    data->last_token = NULL;
    data->token = NULL;
    data->pipe = NULL;
}