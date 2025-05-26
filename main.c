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

// void    skip_quotes(char *s, char quote, int *idx)
// {
//     bool    in_quote;

//     in_quote = true;
//     while (s[*idx] != '\0' && in_quote)
//     {
//         if (s[*idx] == quote)
//             in_quote = false;
//         *idx = *idx + 1;
//     }
// }

// bool    ambs_redirect(t_token *tok)
// {
//     int i;

//     i = 0;
//     skip_spaces(tok->s, &i);
//     if (tok->s[i] == '\0')
//         return (true);
//     while (tok->s[i] != '\0' && !is_space(tok->s[i]))
//     {
//         if (tok->s[i] != '"' || tok->s[i] != '\'')
//             skip_quotes(&(tok->s[i]), tok->s[i], &i);
//         else
//             i++;
//     }
//     skip_spaces(tok->s, &i);
//     if (tok->s[i] != '\0')
//         return (true);
//     return (false);
// }

// bool    remove_quotes(t_token *tok)
// {
//     while (tok)
//     {
//         if (tok->prev && tok->prev->type != PIPE
//             && tok->prev->type != ARGUMENT && tok->prev->type != HERE_DOC)
//         {
//             if (ambs_redirect(tok))
//                 return (false);
//         }

//     }
// }

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
//scrivere adddollar e farre in modo che si setti attach in add dollar e add argument se va fatto 