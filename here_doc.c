/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <lmenoni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 18:03:18 by lmenoni           #+#    #+#             */
/*   Updated: 2025/05/19 17:40:44 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool is_limiter_quoted(char *s)
{
	int i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == '"' || s[i] == '\'')
			return (true);
		i++;
	}
	return (false);
}

char *get_lines(char *s)
{
	char *r;
	char *line;
	char *limiter;

	limiter = get_unquoted(s);
	r = NULL;
	while (1)
	{
		line = NULL;
		line = readline("> ");
		if (line[0] != '\0' && ft_strncmp(line, limiter, ft_strlen(line)) == 0)
		{
			free(line);
			free(limiter);
			return (r);
		}
		if (r)
			r = ft_buffjoin(r, "\n");
		r = ft_buffjoin(r, line);
		free(line);
	}
	return (NULL);
}

void do_here_doc(t_token *tok, t_data *data)
{
	char *r;
	bool	quoted;

	r = NULL;
	while (tok)
	{
		if (tok->type == HERE_DOC)
		{
			quoted = is_limiter_quoted(tok->next->s);
			r = get_lines(tok->next->s);
			if (!quoted)
				r = expand_dollar(r, data, true);
			tok->next->s = r;
		}
		tok = tok->next;
	}
}
