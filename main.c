/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <lmenoni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 16:21:16 by lmenoni           #+#    #+#             */
/*   Updated: 2025/05/15 17:33:27 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    init_env(char **env, t_data *data)
{
	int i;
	t_env *head;

	i = 0;
	data->env_data = malloc(sizeof(t_env));
	if (!data->env_data)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	head = data->env_data;
	while(env[i])
	{
		data->env_data->e = env[i];
		data->env_data->next = malloc(sizeof(t_env));
		if (!data->env_data->next)
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		data->env_data = data->env_data->next;
		i++;
	}
	data->env_data->next = NULL;
	data->env_data = head;
}

int main(int ac, char **av, char **e)
{
    t_data  data;
    (void)ac;
    (void)av;
    data = (t_data){0};
    init_env(e, &data);
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
        //expand_dollar(data.token);
        // prima di creare array fare parsing dei syntax error (senno seg)
            make_cmd_array(&data);
            print_cmd_array(&data);
        }
        add_history(data.input);
        free(data.input);
        free_token(data.token);
        free_cmd_array(&data);
    }
    rl_clear_history();
    return (0);
}
