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

int len_wo_quotes(char *s)
{
    int i;
    int len;

    i = 0;
    len = 0;
    while (s[i] != '\0')
    {
        if ((s[i] == '"' || s[i] == '\'') && !is_quoted(s, &s[i]))
            i++;
        else
        {
            i++;
            len++;
        }
    }
    return (len);
}

char    *get_unquoted(char *s)
{
    int i;
    int j;
    char    *r;

    i = 0;
    j = 0;
    while (s[i] != '\0' && s[i] != '"' && s[i] != '\'')
        i++;
    if (s[i] == '\0')
        return (s);
    r = malloc((len_wo_quotes(s) + 1) * sizeof(char));
    i = 0;
    while (s[i] != '\0')
    {
        if ((s[i] == '"' && is_quoted(s, &s[i]) != 1 ) || (s[i] == '\'' && is_quoted(s, &s[i]) != 2))
            i++;
        else
        {
            r[j] = s[i];
            i++;
            j++;
        }
    }
    r[j] = '\0';
    return (free(s), r);
}

void    remove_quotes(t_token *tok)
{
    while (tok)
    {
        if (tok->type == ARGUMENT && (!tok->prev || (tok->prev && tok->prev->type != HERE_DOC)))
            tok->s = get_unquoted(tok->s);
        tok = tok->next;
    }
}

void    do_quoted(t_token *tok, t_data *data)
{
    while (tok)
    {
        if (tok->type == ARGUMENT && (!tok->prev || (tok->prev && tok->prev->type != HERE_DOC)))
            tok->s = expand_dollar(tok->s, data, false);
        tok = tok->next;
    }
}

void    expand(t_token *tok, t_data *data)
{
    do_quoted(tok, data);
    remove_quotes(tok);
    //do_dollar;
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
        tokenize_input(&data);
        print_tokens(data.token);
        if (!parse_syntax_errors(data.token))
        {
            do_here_doc(data.token, &data);
            print_tokens(data.token);
            expand(data.token, &data);
            make_cmd_array(&data);
            print_cmd_array(&data);
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
//testare la rimozione delle quote (test: "ciao'"' mond"o')
//dopo eseguito gli heredoc espando variabili tra quote rimuovendole prima, poi espando i token dollar splittando argomenti in diversi token e facendo attach del caso e controlli per ambig_redi