/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <lmenoni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 18:14:25 by lmenoni           #+#    #+#             */
/*   Updated: 2025/05/15 14:13:12 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    tokenize_input(t_data *data)
{
    int     i;
    
    i = 0;
    while (data->input[i] != '\0')
    {
        skip_spaces(data->input, &i);
        if (data->input[i] == '|')
            add_pipe(data, &data->input[i], &i);
        else if (data->input[i] == '<' || data->input[i] == '>')
            add_redirect(data, &data->input[i], &i, data->input[i]);
        else
            add_argument(data, &data->input[i], &i);
    }
}
