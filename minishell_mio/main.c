/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 16:21:16 by lmenoni           #+#    #+#             */
/*   Updated: 2025/05/03 17:05:41 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    do_echo(t_data *data)
{
    //da implementare eh eh
    int i = 1;
    while (data->input_array[i])
    {
        ft_printf("|%s|", data->input_array[i]);
        i++;
    }
    // ft_printf("%s", data->input_array[1]);
}

int define_input(t_data *data)
{
    const char *path = getenv("PATH");
    if (!ft_strncmp(data->input, "exit", ft_strlen(data->input)))
        return (0);
    if (!ft_strncmp(data->input, "echo", 4))
        return (do_echo(data), 1);
    if (!ft_strncmp(data->input, "|", 1))
        return (do_pipe(data, path), 1);
    return (1);
}

int main()
{
    t_data  data;
    
    data = (t_data){0};
    while (1)
    {
        data.input = readline(CYAN"minishell"RESET YELLOW">"RESET);
        if (!data.input)
            break ;
        data.input_array = ft_split(data.input, ' ');
        if (!define_input(&data))
        {
            free(data.input);
            ft_freemat((void *)data.input_array, ft_matlen(data.input_array));
            break;
        }
        add_history(data.input);
        free(data.input);
        ft_freemat((void *)data.input_array, ft_matlen(data.input_array));
    }
    rl_clear_history();
    return (0);
}
