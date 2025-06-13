/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 14:19:11 by lmenoni           #+#    #+#             */
/*   Updated: 2025/06/13 14:20:48 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_var_name(char *var)
{
	int i;

	i = 0;
	while (var[i] && (var[i] != '=' && var[i] != '+'))
		i++;
	return (ft_substr(var, 0, i));
}

void print_export(t_env *env, int i)
{
    if (i > 0 && env->e[i] == '=' && ft_strncmp(env->e, "_=", 2) != 0)
    {
        char *var_name = get_var_name(env->e);
        ft_printf("declare -x %s=\"%s\"\n", var_name, &env->e[i + 1]);
        free(var_name);
    }
    else if (ft_strncmp(env->e, "_=", 2) != 0)
        ft_printf("declare -x %s\n", env->e);
}
