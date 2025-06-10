/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 14:09:23 by lmenoni           #+#    #+#             */
/*   Updated: 2025/06/03 14:09:27 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    add_to_token_struct(t_token **tok, t_data *data, t_token *new)
{
    if ((*tok)->prev && (*tok)->prev->type >= 3 && tok_len(new) > 1)
    {
        (*tok)->type = AMB_REDI;
        free_token(new);
        return ;
    }
    else if ((*tok)->prev)
    {
        new->prev = (*tok)->prev;
        (*tok)->prev->next = new;
    }
    else
        data->token = new;
    new->type = ARGUMENT;
    if ((*tok)->next)
    {
        while (new->next)
            new = new->next;
        (*tok)->next->prev = new;
        new->next = (*tok)->next;
    }
    free((*tok)->s);
    free(*tok);
    *tok = new;
}

void    tokenize_dollar(t_token **new, char **arr, int i, t_data *data)
{
    t_token *temp;

    temp = NULL;
    arr[i] = expand_dollar(arr[i], data, false);
    temp = create_temp(arr[i]);
    if (temp == NULL)
        return ;
    if (!(*new))
    {
        *new = temp;
        return ;
    }
    add_to_new(new, arr, i, &temp);
}

void    tokenize_string(t_token **new, char **arr, int i, t_data *data)
{
    t_token *curr;
    t_token *temp;

    curr = *new;
    while (curr && curr->next)
        curr = curr->next;
    arr[i] = expand_dollar(arr[i], data, false);
    arr[i] = get_unquoted(arr[i]);
    if (*new)
    {
        if (!is_space(last_char(arr[i - 1])))
        {
            curr->s = ft_buffjoin(curr->s, arr[i]);
            return ;
        }
    }
    temp = token_new(ft_strdup(arr[i]), ARGUMENT);
    if (!(*new))
    {
        *new = temp;
        return ;
    }
    curr->next = temp;
    temp->prev = curr;
}

void    handle_unquoted_expansion(t_token **tok, t_data *data)
{
    char    **arr;
    int     i;
    t_token *new;

    i = 0;
    new = NULL;
    arr = split_token((*tok)->s);
    while (arr[i] != NULL)
    {
        if (arr[i][0] == '$')
            tokenize_dollar(&new, arr, i, data);
        else
            tokenize_string(&new, arr, i, data);
        i++;
    }
    ft_freemat((void **)arr, ft_matlen(arr));
    add_to_token_struct(tok, data, new);
}

void    expand(t_token *tok, t_data *data)
{
    while (tok)
    {
        if (tok->type == ARGUMENT && (!tok->prev || (tok->prev && tok->prev->type != HERE_DOC)) && is_dollar_quoted(tok->s))
        {
            tok->s = expand_dollar(tok->s, data, false);
            tok->s = get_unquoted(tok->s);
        }
        else if (tok->type == ARGUMENT && (!tok->prev || (tok->prev && tok->prev->type != HERE_DOC)) && !is_dollar_quoted(tok->s))
            handle_unquoted_expansion(&tok, data);
        tok = tok->next;
    }
}
