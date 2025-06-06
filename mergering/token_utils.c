/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <lmenoni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:42:37 by lmenoni           #+#    #+#             */
/*   Updated: 2025/05/19 12:48:14 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *token_new(char *content, tok_type type)
{
	t_token *token;

	token = malloc(1 * sizeof(t_token));
	// protezione malloc
	token->s = content;
	token->next = NULL;
	token->prev = NULL;
	token->type = type;
	return (token);
}

void add_token(char *s, tok_type type, t_data *data)
{
	t_token *new;
	t_token *curr;

	new = token_new(s, type);
	if (!(data->token))
	{
		data->token = new;
		data->last_token = new;
		return ;
	}
	curr = data->token;
	while (curr->next)
		curr = curr->next;
	curr->next = new;
	new->prev = curr;
	data->last_token = new;
	return ;
}

tok_type which_type(char c, int len)
{
	if (c == '<' && len == 2)
		return (HERE_DOC);
	else if (c == '<' && len == 1)
		return (REDI_IN);
	else if (c == '>' && len == 2)
		return (APPEND);
	else if (c == '>' && len == 1)
		return (REDI_OUT);
	return (0);
}
