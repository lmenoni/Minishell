/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 16:21:16 by lmenoni           #+#    #+#             */
/*   Updated: 2025/06/12 19:31:59 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t last_signal = 0;

bool    parsing(t_data *data)
{
    if (parse_quotes(data->input))
    {
        data->status = 2;
        return (false);
    }
    tokenize_input(data);
    //print_tokens(data->token);
    if (parse_syntax_errors(data->token))
    {
        data->status = 2;
        return (false);
    }
    if (!do_here_doc(data->token, data))
        return (false);
    expand(data->token, data);
    // print_tokens(data->token);
    make_cmd_array(data);
    // print_cmd_array(data);
    create_pipe_arr(data);
    return (true);
}

void    execution(t_data *data)
{
    int i;
    pid_t   last_pid;

    i = 0;
    signal(SIGINT, SIG_IGN);
    while (i < data->cmd_count)
    {
        last_pid = execute(data->cmd_arr[i], data);
        data->cmd_name++;
        i++;
    }
    free_pipe(data->pipe, (data->cmd_count - 1));
    wait_status(data, last_pid);
    signal(SIGINT, handle_sigint);
    dup2(data->st_in, STDIN_FILENO);
	dup2(data->st_out, STDOUT_FILENO);
    last_signal = 0;
}

void    init_data(t_data *data, char **e)
{
    (*data) = (t_data){0};
    data->env_data = init_env(e, data);
    if (!check_env(data, "OLDPWD"))
        add_env(data, "OLDPWD");
    init_signals();
    data->st_in = dup(STDIN_FILENO);
    data->st_out = dup(STDOUT_FILENO);
}

int main(int ac, char **av, char **e)
{
    t_data  data;
    (void)ac;
    (void)av;
    init_data(&data, e);
    while (1)
    {
        reset_data(&data);
        data.input = readline(CYAN"minishell"RESET YELLOW">"RESET);
        if (!data.input)
            break ;
        check_last_signal(&data);
        if (parsing(&data))
            execution(&data);
        // ft_printf("%s\n", getcwd(NULL, 0));
        add_history(data.input);
    }
    free_env(data.env_data);
    free(data.current_path);
    free(data.old_path);
    close(data.st_in);
    close(data.st_out);
    rl_clear_history();
    return (0);
}
//segfault espansione variabili che non esistono echo $cazzo
//gestione fallimento pipe array, controllare open funzionino correttamente