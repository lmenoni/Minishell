/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 16:21:16 by lmenoni           #+#    #+#             */
/*   Updated: 2025/06/10 19:10:01 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t last_signal = 0;

int define_input(t_data *data, t_cmd *cmd)
{
	if (!ft_strncmp(cmd->args[0], "echo", 4))
		return(echo(data, cmd, cmd->args), 1);
    else if (!ft_strncmp(cmd->args[0], "cd", 2))
		return(cd(data, cmd, cmd->args), 1);
	else if (!ft_strncmp(cmd->args[0], "pwd", 3))
		return(pwd(data, cmd), 1);
	else if (!ft_strncmp(cmd->args[0], "env", 3))
		return(env(data, cmd, cmd->args), 1);
	else if (!ft_strncmp(cmd->args[0], "unset", 5))
		return(unset(data, cmd, cmd->args), 1);
	else if (!ft_strncmp(cmd->args[0], "export", 6))
		return(export(data, cmd->args), 1);
    else if (!ft_strncmp(cmd->args[0], "exit", 4))
        return(exit_shell(data, cmd), 1);
    return (0);
}

char    **copy_env(t_env *env)
{
    int len;
    char **r;
    t_env *t;
    int     i;

    len = 0;
    i = 0;
    t = env;
    while (t)
    {
        len++;
        t = t->next;
    }
    r = malloc((len + 1) * sizeof(char *));
    t = env;
    while (i < len)
    {
        r[i] = ft_strdup(t->e);
        i++;
        t = t->next;
    }
    r[i] = NULL;
    return (r);
}

void	handle_sigint(int sig)
{
	(void)sig;
    last_signal = 1;
	rl_replace_line("", 0);
	rl_on_new_line();
    ft_printf("\n");
    rl_redisplay();
}

void	init_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

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
    //print_tokens(data->token);
    make_cmd_array(data);
    // print_cmd_array(data);
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
    last_signal = 0;
}

bool    check_last_signal(t_data *data)
{
    if (last_signal)
    {
        last_signal = 0;
        free(data->input);
        data->input = NULL;
        data->status = 130;
        return (true);
    }
    return (false);
}

int main(int ac, char **av, char **e)
{
    t_data  data;
    (void)ac;
    (void)av;
    data = (t_data){0};
    data.env_data = init_env(e, &data);
    init_signals();
    data.st_in = dup(STDIN_FILENO);
    data.st_out = dup(STDOUT_FILENO);
    printf("in: %d, out: %d\n", data.st_in, data.st_out);
    while (1)
    {
        reset_data(&data);
        data.input = readline(CYAN"minishell"RESET YELLOW">"RESET);
        if (!data.input)
            break ;
        check_last_signal(&data);
        if (parsing(&data))
        {
            create_pipe_arr(&data);
            execution(&data);
            dup2(data.st_in, STDIN_FILENO);
	        dup2(data.st_out, STDOUT_FILENO);
        }
        add_history(data.input);
    }
    free_env(data.env_data);
    free(data.current_path);
    rl_clear_history();
    return (0);
}

//gestione fallimento pipe array, controllare open funzionino correttamente