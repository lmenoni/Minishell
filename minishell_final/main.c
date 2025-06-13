/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 16:21:16 by lmenoni           #+#    #+#             */
/*   Updated: 2025/06/12 19:31:59 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_last_signal = 0;

bool	parsing(t_data *data)
{
	if (parse_quotes(data->input))
	{
		data->status = 2;
		return (false);
	}
	tokenize_input(data);
	if (parse_syntax_errors(data->token))
	{
		data->status = 2;
		return (false);
	}
	if (!do_here_doc(data->token, data))
		return (false);
	expand(data->token, data);
	make_cmd_array(data);
	create_pipe_arr(data);
	return (true);
}

void	execution(t_data *data)
{
	int		i;
	pid_t	last_pid;

	i = 0;
	signal(SIGINT, SIG_IGN);
	while (i < data->cmd_count)
	{
		last_pid = execute(data->cmd_arr[i], data);
		data->cmd_name++;
		i++;
	}
	free_pipe(data->pipe, (data->cmd_count - 1));
	wait_status(data, last_pid);
	signal(SIGINT, handle_sigint);
	dup2(data->st_in, STDIN_FILENO);
	dup2(data->st_out, STDOUT_FILENO);
	g_last_signal = 0;
}

int	main(int ac, char **av, char **e)
{
	t_data	data;

	(void)ac;
	(void)av;
	init_data(&data, e);
	while (1)
	{
		reset_data(&data);
		data.input = readline(CYAN"minishell"RESET YELLOW"> "RESET);
		if (!data.input)
			break ;
		check_last_signal(&data);
		if (parsing(&data))
			execution(&data);
		add_history(data.input);
	}
	close_data(&data);
	return (0);
}
