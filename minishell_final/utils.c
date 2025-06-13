/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:46:48 by lmenoni           #+#    #+#             */
/*   Updated: 2025/06/10 17:45:18 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int parse_syntax_errors(t_token *token)
{
    while (token)
    {
        if (token->type == PIPE && !(token->next))
            return (ft_printf_fd(2, "minishell: syntax error near unexpected token `newline'\n"));
        if (token->type == PIPE && (!(token->prev) || token->next->type == PIPE))
            return (ft_printf_fd(2, "minishell: syntax error near unexpected token `|'\n"));
        if (token->type >= 3 && !(token->next))
            return (ft_printf_fd(2, "minishell: syntax error near unexpected token `newline'\n"));
        if (token->type >= 3 && (token->next->type >= 3 ||  token->next->type == PIPE))
            return (ft_printf_fd(2, "minishell: syntax error near unexpected token `>'\n"));
        token = token->next;
    }
    return (0);
}

void    reset_data(t_data *data)
{
    free(data->input);
    free_token(data->token);
    free_cmd_array(data);
    data->cmd_count = 0;
    data->cmd_name = 0;
    data->cmd_arr = NULL;
    data->input = NULL;
    data->last_token = NULL;
    data->token = NULL;
    data->pipe = NULL;
}