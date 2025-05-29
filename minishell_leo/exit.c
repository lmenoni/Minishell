/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:34:47 by igilani           #+#    #+#             */
/*   Updated: 2025/05/29 17:59:11 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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