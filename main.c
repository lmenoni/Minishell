/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <lmenoni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 16:21:16 by lmenoni           #+#    #+#             */
/*   Updated: 2025/05/20 12:48:23 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    skip_quotes(char *s, char quote, int *idx)
{
    bool    in_quote;

    in_quote = true;
    while (s[*idx] != '\0' && in_quote)
    {
        if (s[*idx] == quote)
            in_quote = false;
        *idx = *idx + 1;
    }
}

bool    ambs_redirect(t_token *tok)
{
    int i;
    char    quote;
    bool    in_quote;

    i = 0;
    quote = 0;
    in_quote = false;
    skip_spaces(s, &i);
    if (s[i] == '\0')
        return (true);
    while (tok->s[i] != '\0' && !is_space(tok->s[i]))
    {
        if (tok->s[i] != '"' || tok->s[i] != '\'')
            skip_quotes(&(tok->s[i]), tok->s[i], &i);
        else
            i++;
    }
    skip_spaces(s, &i);
    if (s[i] != '\0')
        return (true);
    return (false)
}

bool    remove_quotes(t_token *tok)
{
    while (tok)
    {
        if (tok->prev && tok->prev->type != PIPE
            && tok->prev->type != ARGUMENT && tok->prev->type != HERE_DOC)
        {
            if (ambs_redirect(tok))
                return (false);
        }

    }
}

void    expand_tokens(t_token *tok, t_data *data)
{
    while (tok)
    {
        if (!tok->prev || tok->prev->type != HERE_DOC)
            tok->s = expand_dollar(tok->s, data);
        tok = tok->next;
    }
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
        //print_tokens(data.token);
        if (!parse_syntax_errors(data.token))
        {
            do_here_doc(data.token, &data);
            expand_tokens(data.token, &data);
            remove_quotes(data.token);
            //print_tokens(data.token);
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
// le variabili che vengono espanse si dividono in nuovi token se contengono spazi tra parole es >< $.. o $a=echo ciao , $a