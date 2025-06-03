/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_tokens_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 14:09:46 by lmenoni           #+#    #+#             */
/*   Updated: 2025/06/03 14:09:47 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    add_to_new(t_token **new, char **arr, int i, t_token **temp)
{
    t_token *last;
    t_token *curr;

    last = *new;
    while (last->next)
        last = last->next;
    if ((last->type != AMB_REDI && !is_space(arr[i][0]))
        || (last->type == AMB_REDI && !is_space(arr[i][0])
        && !is_space(last_char(arr[i - 1]))))
    {
        curr = *temp;
        *temp = (*temp)->next;
        last->s = ft_merge(last->s, curr->s);
        last->type = AMB_REDI;
        free(curr);
    }
    if (*temp)
    {
        last->next = *temp;
        (*temp)->prev = last;
    }
}

t_token *create_temp(char *s)
{
    int j;
    int len;
    char    *str;
    t_token *r;

    j = 0;
    len = 0;
    r = NULL;
    if (s[0] == '\0')
        return (add_temp(ft_strdup(s), &r), r);
    while (s[j] != '\0')
    {
        len = 0;
        skip_spaces(s, &j);
        if (s[j] != '\0')
        {
            while (s[j + len] != '\0' && !is_space(s[j + len]))
                len++;
            str = ft_strndup(&s[j], len);
            add_temp(str, &r);
            j += len;
        }
    }
    return (r);
}

void    add_temp(char *content, t_token **first)
{
    t_token *new;
    t_token *curr;

    new = token_new(content, AMB_REDI);
    if (!(*first))
        *first = new;
    else
    {
        curr = *first;
        while (curr->next)
            curr = curr->next;
        curr->next = new;
        new->prev = curr;
    }
}

void    fill_array(char *t, int *i, int *j, char **r)
{
    if (t[*i] != '$')
    {
        while_string(t, i);
        r[*j] = ft_strndup(t, *i);
        *j = *j + 1;
    }
    else
    {
        if (t[*i + 1] == '"' || t[*i + 1] == '\'' || t[*i + 1] == '$')
            *i = *i + 1;
        else
        {
            while_var(t, i);
            r[*j] = ft_strndup(t, *i);
            *j = *j + 1;
        }
    }
}

char    **split_token(char *s)
{
    int j;
    int i;
    char    **r;
    char    *t;

    i = 0;
    j = 0;
    t = s;
    r = malloc((count_pieces(s) + 1) * sizeof(char *));
    while (t[i] != '\0')
    {
        fill_array(t, &i, &j, r);
        t += i;
        i = 0;
    }
    r[j] = NULL;
    return (r);
}
