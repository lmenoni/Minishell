/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <lmenoni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 18:14:25 by lmenoni           #+#    #+#             */
/*   Updated: 2025/05/19 12:47:03 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	add_dollar(t_data *data, char *s, int *idx, bool attach)
// {
// 	int	i;
// 	int	len;
// 	char *r;

// 	i = 1;
// 	len = 0;
// 	//ft_printf("adding dollar\n");
// 	while (s[i] != '\0' && !is_space(s[i]) && s[i] != '"'
// 		&& s[i] != '\'' && s[i] != '$' && s[i] != '|'
// 		&& s[i] != '<' && s[i] != '>')
// 		i++;
// 	len = i;
// 	r = malloc((len + 1) * sizeof(char));
// 	i = 0;
// 	while (i < len)
// 	{
// 		r[i] = s[i];
// 		i++;
// 		*idx = *idx + 1;
// 	}
// 	r[i] = '\0';
// 	add_token(r, DOLLAR, data, attach);
// }

int argument_len(char *s)
{
	int i;
	char quote;
	bool in_quote;

	i = 0;
	quote = 0;
	in_quote = false;
	while (in_quote || (!in_quote && s[i] != '\0'
		&& s[i] != ' ' && s[i] != '|' && s[i] != '<' && s[i] != '>'))
	{
		if ((!in_quote && (s[i] == '"' || s[i] == '\'')) || quote == s[i])
		{
			in_quote = !in_quote;
			quote = s[i];
		}
		i++;
	}
	return (i);
}

void add_argument(t_data *data, char *s, int *idx)
{
	int i;
	int len;
	char *r;

	i = 0;
	len = argument_len(s);
	r = malloc((len + 1) * sizeof(char));
	// copertura malloc
	while (i < len)
	{
		r[i] = s[i];
		i++;
		*idx = *idx + 1;
	}
	r[i] = '\0';
	add_token(r, ARGUMENT, data);
}

void add_redirect(t_data *data, char *s, int *idx, char c)
{
	int i;
	int len;
	char *r;

	i = 0;
	len = 0;
	while (s[i] != '\0' && s[i] == c && i < 2)
		i++;
	len = i;
	r = malloc((len + 1) * sizeof(char));
	// copertura malloc
	i = 0;
	while (i < len)
	{
		r[i] = s[i];
		i++;
		*idx = *idx + 1;
	}
	r[i] = '\0';
	add_token(r, which_type(c, len), data);
}

void add_pipe(t_data *data, int *idx)
{
	char *r;

	r = malloc((2) * sizeof(char));
	// copertura malloc
	r[0] = '|';
	r[1] = '\0';
	*idx = *idx + 1;
	add_token(r, PIPE, data);
}

void tokenize_input(t_data *data)
{
	int i;

	i = 0;
	while (data->input[i] != '\0')
	{
		skip_spaces(data->input, &i);
		if (data->input[i] != '\0' && data->input[i] == '|')
			add_pipe(data, &i);
		else if (data->input[i] != '\0' && (data->input[i] == '<' || data->input[i] == '>'))
			add_redirect(data, &data->input[i], &i, data->input[i]);
		else if (data->input[i] != '\0')
			add_argument(data, &data->input[i], &i);
	}
}
