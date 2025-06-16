/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_tokens_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 14:09:46 by lmenoni           #+#    #+#             */
/*   Updated: 2025/06/03 14:09:47 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_to_new(t_token **new, t_token **c, t_token **temp)
{
	t_token	*last;
	t_token	*curr;

	last = *new;
	while (last->next)
		last = last->next;
	if ((*c)->prev && ((last->type != AMB_REDI && !is_space((*c)->s[0]))
			|| (last->type == AMB_REDI && !is_space((*c)->s[0])
				&& !is_space(last_char((*c)->prev->s)))))
	{
		curr = *temp;
		*temp = (*temp)->next;
		last->s = ft_merge(last->s, curr->s);
		last->type = AMB_REDI;
		free(curr);
	}
	if (*temp)
	{
		last->next = *temp;
		(*temp)->prev = last;
	}
}

t_token	*create_temp(char *s)
{
	int		j;
	int		len;
	char	*str;
	t_token	*r;

	j = 0;
	len = 0;
	r = NULL;
	while (s[j] != '\0')
	{
		len = 0;
		skip_spaces(s, &j);
		if (s[j] != '\0')
		{
			while (s[j + len] != '\0' && !is_space(s[j + len]))
				len++;
			str = ft_strndup(&s[j], len);
			add_temp(str, &r, AMB_REDI);
			j += len;
		}
	}
	return (r);
}

void	add_temp(char *content, t_token **first, t_type type)
{
	t_token	*new;
	t_token	*curr;

	new = token_new(content, type);
	if (!(*first))
		*first = new;
	else
	{
		curr = *first;
		while (curr->next)
			curr = curr->next;
		curr->next = new;
		new->prev = curr;
	}
}

void	fill_array(char *t, int *i, t_token **r)
{
	if (t[*i] != '$')
	{
		while_string(t, i);
		add_temp(ft_strndup(t, *i), r, ARGUMENT);
	}
	else
	{
		if (t[*i + 1] == '"' || t[*i + 1] == '\'' || t[*i + 1] == '$')
			*i = *i + 1;
		else
		{
			while_var(t, i);
			add_temp(ft_strndup(t, *i), r, AMB_REDI);
		}
	}
}

t_token	*split_token(char *s)
{
	t_token	*r;
	int		i;
	char	*t;

	t = s;
	r = NULL;
	i = 0;
	while (t[i] != '\0')
	{
		fill_array(t, &i, &r);
		t += i;
		i = 0;
	}
	return (r);
}
