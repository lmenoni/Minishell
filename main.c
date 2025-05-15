/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <lmenoni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 16:21:16 by lmenoni           #+#    #+#             */
/*   Updated: 2025/05/14 14:30:29 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int name_len(char *s)
// {
//     int i;
    
//     i = 0;
//     while (!(is_space(s[i])))
//         i++;
//     return (i);
// }

// void    expand_quoted(t_token *curr)
// {
//     int i;
//     int len;
//     int name;

//     i = 0;
//     len = ft_strlen(curr->s);
//     name = 0;
//     while (curr->s[i] && curr->s[i] != '$')
//         i++;
//     name = name_len(&curr->s);
//     i++;
//     if (name > 1)
    
// }

// void    expand_dollar(t_token *token)
// {
//     t_token *curr;

//     curr = token;
//     while (curr)
//     {
//         if (curr->type != STRING)
//             expand_quoted(curr);
//         curr = curr->next;
//     }
// }

void    reset_data(t_data *data)
{
    data->cmd_count = 0;
    data->cmd_arr = NULL;
    data->input = NULL;
    data->last_token = NULL;
    data->token = NULL;
}

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
        //expand_dollar(data.token);
        make_cmd_array(&data);
        print_cmd_array(&data);
        add_history(data.input);
        free(data.input);
        free_token(data.token);
        free_cmd_array(&data);
    }
    rl_clear_history();
    return (0);
}
