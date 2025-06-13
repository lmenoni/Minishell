/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 18:03:18 by lmenoni           #+#    #+#             */
/*   Updated: 2025/06/10 17:43:20 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_limiter_quoted(char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == '"' || s[i] == '\'')
			return (true);
		i++;
	}
	return (false);
}

void	eof_check(char *line, char *limiter, char *prompt)
{
	if (!line)
		ft_printf(ERR_EOF, limiter);
	if (line)
		free(line);
	free(limiter);
	free(prompt);
}

char	*get_lines(char *s, t_data *data)
{
	char	*r;
	char	*line;
	char	*limiter;
	char	*prompt;

	limiter = get_unquoted(s);
	prompt = ft_crazystring(limiter);
	r = NULL;
	while (1)
	{
		line = NULL;
		line = readline(prompt);
		if (check_lstsig_here(line, limiter, r, data))
			break ;
		if (r)
			r = ft_buffjoin(r, "\n");
		if (!line || (limiter[0] == '\0' && line[0] == '\0')
			|| (line[0] != '\0'
				&& ft_strncmp(line, limiter, ft_strlen(line)) == 0))
			return (eof_check(line, limiter, prompt), r);
		r = ft_buffjoin(r, line);
		free(line);
	}
	free(prompt);
	return (NULL);
}

bool	do_here_doc(t_token *tok, t_data *data)
{
	char	*r;
	bool	quoted;

	r = NULL;
	signal(SIGINT, here_sigint);
	while (tok && g_last_signal == 0)
	{
		if (tok->type == HERE_DOC && g_last_signal == 0)
		{
			quoted = is_limiter_quoted(tok->next->s);
			r = get_lines(tok->next->s, data);
			if (!quoted && r)
				r = expand_dollar(r, data, true);
			else if (!r)
				r = ft_strdup("\0");
			tok->next->s = r;
		}
		tok = tok->next;
	}
	return (reset_signal());
}
