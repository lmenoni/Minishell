/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:34:47 by igilani           #+#    #+#             */
/*   Updated: 2025/06/05 18:56:36 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
| Comando        | Esce?  | Exit code | Note                               |
| -------------- | ------ | --------- | ---------------------------------- |
| `exit 32 32`   | ❌     | -         | ❌ Troppi argomenti                |
| `exit abc`     | ✅     | 2       | ❗ Argomento non numerico          |
| `exit "32 32"` | ✅     | 2       | ❗ Stringa non numerica con spazio |
| `exit`         | ✅     | Ultimo    | Uscita normale                     |
| `exit 0`       | ✅     | 0         | Successo                           |
| `exit 1`       | ✅     | 1         | Errore                             |
*/

#include "minishell.h"

// int parse_exit(char **args)
// {
// 	int i;

// 	// (ft_atoll(args[0]) > LONG_MAX || ft_atoll(args[0]) < LONG_MIN || i > 20)
// 	while (*args)
// 	{
// 		if (!*args)
// 			return (0);
// 		else if (ft_matlen(args) > 1)
// 		{
// 			ft_printf("minishell: exit: too many arguments\n");
// 			return (1);
// 		}
// 		else if (ft_matlen(args) == 1)
// 		{
// 			i = 0;
// 			while (args[0][i])
// 			{
// 				if (ft_isalpha(args[0][i]) || args[0][i] == ' ') // ft_alphanum
// 				{
// 					ft_printf_fd(2, "minishell: exit: %s: numeric argument required\n", args[0]);
// 					return (255);
// 				}
// 				i++;
// 			}
// 			if (ft_atoi(args[0]) < 0 || ft_atoi(args[0]) > 255)
// 				return ((unsigned char)ft_atoi(args[0]));
// 			return (ft_atoi(args[0]));
// 		}
// 	}
// 	return (0);
// }

// int parse_exit(char **args)
// {
// 	int i;

// 	if (!args || !*args)
// 		return (0);
// 	if (ft_matlen(args) > 1)
// 	{
// 		ft_printf("minishell: exit: too many arguments\n");
// 		return (1);
// 	}	
// 	i = 0;
// 	skip_spaces(args[0], &i);
// 	while (args[0][i] != '\0' && args[0][i] != ' ')
// 		i++;
// 	skip_spaces(args[0], &i);
// 	if (args[0][i] != '\0')
// 	{
// 		ft_printf_fd(2, "minishell: exit: %s: numeric argument required\n", args[0]);
// 		return (2);
// 	}
// 	if (atoll(args[0]) <= LONG_MIN || atoll(args[0]) >= LONG_MAX || i > 20)
// 	{
// 		ft_printf_fd(2, "minishell: exit: %s: numeric argument required\n", args[0]);
// 		return (2);
// 	}
// 	if (isdigit(args[0][0]) == 0 && args[0][0] != '-' && args[0][0] != '+')
// 	{
// 		ft_printf_fd(2, "minishell: exit: %s: numeric argument required\n", args[0]);
// 		return (2);
// 	}
// 	return ((unsigned char)atoll(args[0]));
// }

void exit_execution(t_data *data, t_cmd *cmd)
{
	if (!cmd->pipe_in && !cmd->pipe_out)
		ft_printf("exit\n");
	exit(data->exit_status);
}

int validate_exit_arg(char *arg)
{
    int i = 0;

    skip_spaces(arg, &i);
    while (arg[i] != '\0' && arg[i] != ' ')
        i++;
    skip_spaces(arg, &i);
    if (arg[i] != '\0')
        return (2);
    if (atoll(arg) <= LONG_MIN || atoll(arg) >= LONG_MAX || i > 20)
        return (2);
    if (isdigit(arg[0]) == 0 && arg[0] != '-' && arg[0] != '+')
        return (2);
    return ((unsigned char)atoll(arg));
}

int parse_exit(char **args)
{
    int result;

	if (!args || !*args)
		return (0);
	result = validate_exit_arg(args[0]);
	if (result == 2)
		ft_printf_fd(2, "minishell: exit: %s: numeric argument required\n", args[0]);
	else if (ft_matlen(args) > 1)
    {
        ft_printf("minishell: exit: too many arguments\n");
        return (1);
    }
    return (result);
}

void exit_shell(t_data *data, t_cmd *cmd)
{
	(void)data;
    char **args;
    int parsing_result;

	args = cmd->args;
	parsing_result = parse_exit(args + 1);
	ft_printf("%d\n", parsing_result);
}