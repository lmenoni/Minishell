/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <lmenoni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 16:21:16 by lmenoni           #+#    #+#             */
/*   Updated: 2025/05/21 18:02:57 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    while_var(char *s, int *i)
{
    *i = *i + 1;
    while(s[*i] != '\0' && s[*i] != '$' && s[*i] != '"' && s[*i] != '\'')
        *i = *i + 1;
}

void    while_string(char *s, int *i)
{
    while (s[*i] != '\0')
    {
        if (s[*i] == '$' && s[*i + 1] != '\0' && !is_space(s[*i + 1]) && is_quoted(s, &s[*i]) == 0)
            break ;
        *i = *i + 1;
    }
}

int count_pieces(char *s)
{
    int i;
    int r;

    i = 0;
    r = 0;
    while (s[i] != '\0')
    {
        if (s[i] != '$')
        {
            while_string(s, &i);
            r++;
        }
        else
        {
            if (s[i + 1] == '"' || s[i + 1] == '\'' || s[i + 1] == '$')
                i++;
            else
            {
                while_var(s, &i);
                r++;
            }
        }
    }
    return (r);
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

char    last_char(char *s)
{
    if (*s == '\0')
        return ('\0');
    while (*(s + 1) != '\0')
        s++;
    return (*s);
}

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

bool    is_dollar_quoted(char *s)
{
    int i;

    i = 0;
    while (s[i] != '\0')
    {
        if (s[i] == '$' && s[i + 1] != '\0' && !is_space(s[i + 1]) && is_quoted(s, &s[i]) == 0)
            return (false);
        i++;
    }
    return (true);
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

bool    parsing(t_data *data)
{
    if (parse_quotes(data->input))
        return (false);
    tokenize_input(data);
    //print_tokens(data->token);
    if (parse_syntax_errors(data->token))
        return (false);
    do_here_doc(data->token, data);
    expand(data->token, data);
    //print_tokens(data->token);
    make_cmd_array(data);
    print_cmd_array(data);
    return (true);
}

int main(int ac, char **av, char **e)
{
    t_data  data;
    (void)ac;
    (void)av;
    data = (t_data){0};
    data.env_data = init_env(e, &data);
    //print_env(data.env_data);
    while (1)
    {
        reset_data(&data);
        data.input = readline(CYAN"minishell"RESET YELLOW">"RESET);
        if (!data.input)
            break ;
        if (parsing(&data))
        {
            ft_printf("READY FOR EXECUTE\n");
        }
        add_history(data.input);
        free(data.input);
        free_token(data.token);
        free_cmd_array(&data);
    }
    free_env(data.env_data);
    free(data.curr_path);
    rl_clear_history();
    return (0);
}

//CORREGGERE HERE_DOC CHIUSO SUBITO CON LIMITER, ESPANSIONE SEGFOULTA