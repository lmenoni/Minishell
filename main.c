/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 16:21:16 by lmenoni           #+#    #+#             */
/*   Updated: 2025/06/06 17:59:17 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t last_signal = 0;

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
    close(0);
    last_signal = 1;
    // rl_done = 1;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
    // rl_redisplay();
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
    //print_cmd_array(data);
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
        dup2(data->st_in, STDIN_FILENO);
        free(data->input);
        data->input = NULL;
        data->status = 130;
        return (true);
    }
    return (false);
}

// void flush_stdin(void)
// {
//     int bytes;
//     char c;

//     ioctl(STDIN_FILENO, FIONREAD, &bytes);
//     while (bytes-- > 0)
//         read(STDIN_FILENO, &c, 1);
// }

int main(int ac, char **av, char **e)
{
    t_data  data;
    (void)ac;
    (void)av;
    data = (t_data){0};
    data.env_data = init_env(e, &data);
    data.st_in = dup(STDIN_FILENO);
    init_signals();
    //print_env(data.env_data);
    while (1)
    {
        reset_data(&data);
        // flush_stdin();
        data.input = readline(CYAN"minishell"RESET YELLOW">"RESET);
        if (!data.input && last_signal == 0)
            break ;
        if (check_last_signal(&data))
            continue ;
        if (parsing(&data))
        {
            //ft_printf("READY FOR EXECUTE\n");
            create_pipe_arr(&data);
            execution(&data);
        }
        add_history(data.input);
        free(data.input);
        free_token(data.token);
        free_cmd_array(&data);
    }
    free_env(data.env_data);
    free(data.curr_path);
    close(data.st_in);
    rl_clear_history();
    return (0);
}

//gestione signal in here_doc
//gestione fallimento pipe array, controllare open funzionino correttamente