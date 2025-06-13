/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 14:09:23 by lmenoni           #+#    #+#             */
/*   Updated: 2025/06/03 14:09:27 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_to_token_struct(t_token **tok, t_data *data, t_token *new)
{
	if (!check_tok(tok, new, data))
		return ;
	if ((*tok)->prev)
	{
		new->prev = (*tok)->prev;
		(*tok)->prev->next = new;
	}
	else
		data->token = new;
	new->type = ARGUMENT;
	while (new->next)
		new = new->next;
	if ((*tok)->next)
	{
		(*tok)->next->prev = new;
		new->next = (*tok)->next;
	}
	free((*tok)->s);
	free(*tok);
	*tok = new->next;
}

void	tokenize_dollar(t_token **new, t_token **c, t_token **t, t_data *data)
{
	t_token	*temp;

	temp = NULL;
	(*c)->s = expand_dollar((*c)->s, data, false);
	if ((*c)->s[0] == '\0')
		return (delete_node(c, t));
	temp = create_temp((*c)->s);
	if (temp == NULL)
		return ;
	if (!(*new))
	{
		*new = temp;
		(*c) = (*c)->next;
		return ;
	}
	add_to_new(new, c, &temp);
	(*c) = (*c)->next;
}

void	tokenize_string(t_token **new, t_token **c, t_data *data)
{
	t_token	*curr;
	t_token	*temp;

	curr = *new;
	while (curr && curr->next)
		curr = curr->next;
	(*c)->s = expand_dollar((*c)->s, data, false);
	(*c)->s = get_unquoted((*c)->s);
	if (*new && (*c)->prev && !is_space(last_char((*c)->prev->s)))
	{
		curr->s = ft_buffjoin(curr->s, (*c)->s);
		*c = (*c)->next;
		return ;
	}
	temp = token_new(ft_strdup((*c)->s), ARGUMENT);
	if (!(*new))
	{
		*new = temp;
		*c = (*c)->next;
		return ;
	}
	curr->next = temp;
	temp->prev = curr;
	*c = (*c)->next;
}

void	handle_unquoted_expansion(t_token **tok, t_data *data)
{
	t_token	*t;
	t_token	*curr;
	t_token	*new;

	new = NULL;
	t = split_token((*tok)->s);
	curr = t;
	while (curr)
	{
		if (curr->s[0] == '$')
			tokenize_dollar(&new, &curr, &t, data);
		else
			tokenize_string(&new, &curr, data);
	}
	free_token(t);
	add_to_token_struct(tok, data, new);
}

void	expand(t_token *tok, t_data *data)
{
	while (tok)
	{
		if (tok->type == ARGUMENT && (!tok->prev
				|| (tok->prev && tok->prev->type != HERE_DOC))
			&& is_dollar_quoted(tok->s))
		{
			tok->s = expand_dollar(tok->s, data, false);
			tok->s = get_unquoted(tok->s);
			tok = tok->next;
		}
		else if (tok->type == ARGUMENT && (!tok->prev
				|| (tok->prev && tok->prev->type != HERE_DOC))
			&& !is_dollar_quoted(tok->s))
			handle_unquoted_expansion(&tok, data);
		else
			tok = tok->next;
	}
}
