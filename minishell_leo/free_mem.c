/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_mem.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <lmenoni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:45:21 by lmenoni           #+#    #+#             */
/*   Updated: 2025/04/10 18:54:40 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    free_token(t_token *token)
{
    t_token *temp;

    while (token)
    {
        temp = token;
        token = token->next;
        free(temp->s);
        free(temp);
    }
}
