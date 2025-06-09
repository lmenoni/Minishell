/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:34:47 by igilani           #+#    #+#             */
/*   Updated: 2025/06/03 17:47:48 by igilani          ###   ########.fr       */
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
*/

#include "minishell.h"

void exit_execution(int exit_status)
{
    ft_printf("exit\n");
    exit(exit_status);
}

void exit_shell(t_data *data, char **args)
{
    if (args[1] == NULL)
    {
        data->exit_status = 0;
        return (exit_execution(data->exit_status));
    }
    else if (args[2] != NULL)
    {
        print_error("bash: exit: too many arguments\n");
        data->exit_status = 1;
        return (exit_execution(data->exit_status));
    }
}