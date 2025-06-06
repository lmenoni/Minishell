/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <lmenoni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 17:50:19 by lmenoni           #+#    #+#             */
/*   Updated: 2025/05/16 18:13:56 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env   *new_env_node(char *s)
{
    t_env   *new;
    
    new = malloc(sizeof(t_env));
    new->e = s;
    new->next = NULL;
    return (new);
}

t_env   *init_env(char **e, t_data *data)
{
    t_env   *first;
    t_env   *curr;
    t_env   *new;
    int     i;

    first = NULL;
    i = 0;
    while (e[i])
    {
        curr = first;
        new = new_env_node(ft_strdup(e[i]));
        if (!first)
            first = new;
        else
        {
            while (curr->next)
                curr = curr->next;
            curr->next = new;
        }
        i++;
    }
    data->curr_path = getcwd(NULL, 4096);
    data->home_path = getenv("HOME");
    data->old_path = NULL;
    return (first);
}
