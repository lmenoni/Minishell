/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:34:47 by igilani           #+#    #+#             */
/*   Updated: 2025/06/04 19:42:16 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
| Comando        | Esce?  | Exit code | Note                               |
| -------------- | ------ | --------- | ---------------------------------- |
| `exit`         | ✅     | Ultimo    | Uscita normale                     |
| `exit 0`       | ✅     | 0         | Successo                           |
| `exit 1`       | ✅     | 1         | Errore                             |
| `exit 32 32`   | ❌     | -         | ❌ Troppi argomenti                |
| `exit abc`     | ✅     | 255       | ❗ Argomento non numerico          |
| `exit "32 32"` | ✅     | 255       | ❗ Stringa non numerica con spazio |
| `exit (32)`    | ❌     | -         | ❌ Errore di sintassi              |
| `exit 32 abc`  | ❌     | -         | ❌ Troppi argomenti                |
*/

#include "minishell.h"

// void exit_execution(t_data *data, t_cmd *cmd)
// {
// 	if (!cmd->pipe_in && !cmd->pipe_out)
// 		ft_printf("exit\n");
// 	exit(data->exit_status);
// }

int parse_exit(char **args)
{
	
}

void exit_shell(t_data *data, t_cmd *cmd)
{
	(void)data;
    char **args;
    int parsing_result;
	
	args = cmd->args;
	parsing_result = parse_exit(args + 1);
}