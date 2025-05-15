/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <lmenoni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:42:37 by lmenoni           #+#    #+#             */
/*   Updated: 2025/04/28 16:40:32 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *token_new(char *content, tok_type type)
{
    t_token *token;

    token = malloc(1 * sizeof(t_token));
    // protezione malloc
    token->s = content;
    token->next = NULL;
    token->prev = NULL;
    token->type = type;
    return (token);
}

void    add_token(t_token **first, char *s, tok_type type, t_data *data)
{
    t_token *new;
    t_token *curr;

    new = token_new(s, type);
    if (!(*first))
    {
        *first = new;
        data->last_token = new;
        return ;
    }
    curr = *first;
    while (curr->next)
        curr = curr->next;
    curr->next = new;
    new->prev = curr;
    data->last_token = new;
    return ;
}

void    print_tokens(t_token *token)
{
    while (token)
    {
        ft_printf("String is (%s) of tipe (%d)\n", token->s, token->type);
        token = token->next;
    }
}