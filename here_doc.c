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
			return (false);
		i++;
	}
	return (true);
}

int limiter_len(char *s)
{
	int i;
	int len;
	char quote;
	bool in_quote;

	i = 0;
	quote = 0;
	len = 0;
	in_quote = false;
	while (s[i] != '\0')
	{
		if ((!in_quote && (s[i] == '"' || s[i] == '\'')) || quote == s[i])
		{
			in_quote = !in_quote;
			quote = s[i];
			i++;
		}
		else
		{
			i++;
			len++;
		}
	}
	return (len);
}

void fill_limiter(char **r, char *s)
{
	int i;
	int j;
	char quote;
	bool in_quote;

	i = 0;
	quote = 0;
	j = 0;
	in_quote = false;
	while (s[i] != '\0')
	{
		if ((!in_quote && (s[i] == '"' || s[i] == '\'')) || quote == s[i])
		{
			in_quote = !in_quote;
			quote = s[i];
			i++;
		}
		else
		{
			(*r)[j] = s[i];
			i++;
			j++;
		}
	}
	(*r)[j] = '\0';
}

char *get_limiter(char *s)
{
	int len;
	char *r;

	len = limiter_len(s);
	r = malloc((len + 1) * sizeof(char));
	fill_limiter(&r, s);
	return (r);
}

char *get_lines(char *s)
{
	char *r;
	char *line;
	char *limiter;

	limiter = get_limiter(s);
	r = NULL;
	while (1)
	{
		line = NULL;
		line = readline(">");
		if (ft_strncmp(line, limiter, ft_strlen(line)) == 0)
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

	r = NULL;
	while (tok)
	{
		if (tok->type == HERE_DOC)
		{
			r = get_lines(tok->next->s);
			ft_printf("HERE DOC IS %s\n", r);
			if (is_limiter_quoted(tok->next->s))
				r = expand_dollar(r, data);
			free(tok->next->s);
			tok->next->s = r;
		}
		tok = tok->next;
	}
}
