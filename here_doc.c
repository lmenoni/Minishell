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

void    here_sigint(int sig)
{
    (void)sig;
    close(0);
    last_signal = 1;
	write(1, "\n", 1);
}

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

bool	check_lstsig_here(char *line, char *limiter, char *r, t_data *data)
{
	if (last_signal == 1)
	{
		if (r)
			free(r);
		free(line);
		free(limiter);
		dup2(data->st_in, STDIN_FILENO);
		data->status = 130;
		return (true);
	}
	return (false);
}

char *get_lines(char *s, t_data *data)
{
	char *r;
	char *line;
	char *limiter;

	limiter = get_unquoted(s);
	r = NULL;
	while (1)
	{
		// in caso di eof (cntrl d) deve ritornare r valido, se r NULL deve essere una stringa vuota (inoltre dare messaggio di errore)
		line = NULL;
		line = readline("> ");
		if (check_lstsig_here(line, limiter, r, data))
			break ;
		// se il limiter è stringa vuota ("") non deve fare il controllo di line[0]
		if (!line || (line[0] != '\0' && ft_strncmp(line, limiter, ft_strlen(line)) == 0))
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

bool	reset_signal()
{
	if (last_signal)
	{
		last_signal = 0;
		return (false);
	}
	return (true);
}

bool	do_here_doc(t_token *tok, t_data *data)
{
	char *r;
	bool	quoted;

	r = NULL;
	signal(SIGINT, here_sigint);
	while (tok && last_signal == 0)
	{
		if (tok->type == HERE_DOC && last_signal == 0)
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
	signal(SIGINT, handle_sigint);
	return (reset_signal());
}
