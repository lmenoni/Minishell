/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 12:57:42 by lmenoni           #+#    #+#             */
/*   Updated: 2025/06/14 22:55:01 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_tok(t_token **tok, t_token *new, t_data *data)
{
	t_token	*temp;

	if ((*tok)->prev && (*tok)->prev->type >= 3 && (!new || tok_len(new) != 1))
	{
		(*tok)->type = AMB_REDI;
		return (free_token(new), false);
	}
	if (!new)
	{
		temp = *tok;
		*tok = (*tok)->next;
		if (*tok)
			(*tok)->prev = NULL;
		if (temp->prev)
		{
			if (*tok)
				(*tok)->prev = temp->prev;
			temp->prev->next = *tok;
		}
		else
			data->token = *tok;
		return (free(temp->s), free(temp), false);
	}
	return (true);
}

int	tok_len(t_token *tok)
{
	int	len;

	len = 0;
	while (tok)
	{
		tok = tok->next;
		len++;
	}
	return (len);
}

char	*get_unquoted(char *s)
{
	int		i;
	int		j;
	char	*r;

	i = 0;
	j = 0;
	if (!is_limiter_quoted(s))
		return (s);
	r = malloc((len_wo_quotes(s) + 1) * sizeof(char));
	while (s[i] != '\0')
	{
		if ((s[i] == '"' && is_quoted(s, &s[i]) != 1)
			|| (s[i] == '\'' && is_quoted(s, &s[i]) != 2))
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

int	len_wo_quotes(char *s)
{
	int	i;
	int	len;

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

int	parse_quotes(char *input)
{
	int	i;

	i = 0;
	while (input[i] != '\0')
		i++;
	if (is_quoted(input, &input[i]) != 0)
		return (ft_printf_fd(2, "minishell: syntax error quotes left open\n"));
	return (0);
}
