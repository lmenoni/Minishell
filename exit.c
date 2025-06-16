/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:34:47 by igilani           #+#    #+#             */
/*   Updated: 2025/06/10 17:50:03 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_execution(int result, t_cmd *cmd, t_data *data)
{
	if (!cmd->pipe_in && !cmd->pipe_out)
		ft_printf_fd(2, "exit\n");
	free_exit(data, cmd, result);
}

bool	check_numeric(char *arg, int *digitc)
{
	int	i;

	i = 0;
	skip_spaces(arg, &i);
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	while (arg[i] != '\0' && arg[i] != ' ')
	{
		if (!ft_isdigit(arg[i]))
			return (false);
		(*digitc)++;
		i++;
	}
	skip_spaces(arg, &i);
	if (arg[i] != '\0')
		return (false);
	return (true);
}

bool	validate_exit_arg(char *arg, int *result)
{
	int	digitc;

	digitc = 0;
	if (!check_numeric(arg, &digitc) || long_overflow(arg))
	{
		*result = 2;
		return (false);
	}
	*result = (unsigned char)ft_atoll(arg);
	return (true);
}

int	parse_exit(char **args, bool *exec)
{
	int	result;

	if (!args || !*args)
		return (0);
	result = 0;
	if (!validate_exit_arg(args[0], &result))
		ft_printf_fd(2, EXIT_NUM, args[0]);
	else if (ft_matlen(args) > 1)
	{
		ft_printf_fd(2, EXIT_ARG);
		*exec = false;
		return (1);
	}
	return (result);
}

void	exit_shell(t_data *data, t_cmd *cmd)
{
	char	**args;
	int		result;
	bool	exec;

	exec = true;
	args = cmd->args;
	result = parse_exit(args + 1, &exec);
	if (exec)
		exit_execution(result, cmd, data);
	if (cmd->pipe_in || cmd->pipe_out)
		exit (result);
	data->status = result;
}
