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

void    do_dollar(t_token *tok, t_data *data)
{

}

void    do_quoted(t_token *tok, t_data *data)
{
    while (tok)
    {
        if (tok->type == ARGUMENT && (!tok->prev || (tok->prev && tok->prev->type != HERE_DOC)))
        {
            tok->s = expand_dollar(tok->s, data, false);
            tok->s = get_unquoted(tok->s);
        }
        tok = tok->next;
    }
}

void    expand(t_token *tok, t_data *data)
{
    do_quoted(tok, data);
    do_dollar(tok, data);
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
        if (!parse_quotes(data.input))
        {
            tokenize_input(&data);
            print_tokens(data.token);
            if (!parse_syntax_errors(data.token))
            {
                do_here_doc(data.token, &data);
                expand(data.token, &data);
                print_tokens(data.token);
                make_cmd_array(&data);
                print_cmd_array(&data);
            }
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
//parsing dell'open quote
//provare a vedere se here doc si puo fare dopo aver espanso etolto le quote, stando attenti ovviamente a non espandere il limiter, obbiettivo ottimizzare visto che la funzione get limiter e limiter len non hanno senso
//poi espando i token dollar splittando argomenti in diversi token e facendo attach del caso e controlli per ambig_redi