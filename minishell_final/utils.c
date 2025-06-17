/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:46:48 by lmenoni           #+#    #+#             */
/*   Updated: 2025/06/10 17:45:18 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_data(t_data *data, char **e)
{
	(*data) = (t_data){0};
	data->env_data = init_env(e, data);
	if (!check_env(data, "OLDPWD"))
		add_env(data, "OLDPWD");
	init_signals();
	data->st_in = dup(STDIN_FILENO);
	data->st_out = dup(STDOUT_FILENO);
}

void	close_data(t_data *data)
{
	free_env(data->env_data);
	free(data->current_path);
	free(data->old_path);
	close(data->st_in);
	close(data->st_out);
	rl_clear_history();
}

int	parse_syntax_errors(t_token *token)
{
	while (token)
	{
		if (token->type == PIPE && !(token->next))
			return (ft_printf_fd(2, SYN_NEWLINE));
		if (token->type == PIPE && (!(token->prev)
				|| token->next->type == PIPE))
			return (ft_printf_fd(2, SYN_PIPE));
		if (token->type >= 3 && !(token->next))
			return (ft_printf_fd(2, SYN_NEWLINE));
		if (token->type >= 3 && (token->next->type >= 3
				|| token->next->type == PIPE))
			return (ft_printf_fd(2, SYN_REDI, token->s));
		token = token->next;
	}
	return (0);
}

void	reset_data(t_data *data)
{
	if (data->input)
		free(data->input);
	if (data->token)
		free_token(data->token);
	if (data->cmd_arr)
		free_cmd_array(data);
	data->cmd_count = 0;
	data->cmd_name = 0;
	data->cmd_arr = NULL;
	data->input = NULL;
	data->last_token = NULL;
	data->token = NULL;
	data->pipe = NULL;
}
