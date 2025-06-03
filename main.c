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

bool    parsing(t_data *data)
{
    if (parse_quotes(data->input))
        return (false);
    tokenize_input(data);
    //print_tokens(data->token);
    if (parse_syntax_errors(data->token))
        return (false);
    do_here_doc(data->token, data);
    expand(data->token, data);
    //print_tokens(data->token);
    make_cmd_array(data);
    print_cmd_array(data);
    return (true);
}

bool    open_out(t_flist *t, t_cmd *cmd)
{
    int fd;

    fd = 0;
    if (t->x_factor)
        fd = open(t->s, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else
        fd = open(t->s, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
        return (ft_printf("minishell: %s: ", t->s), perror(""), false);
    cmd->ou_fd = fd;
    return (true);
}

bool    open_in(t_flist *t, t_cmd *cmd)
{
    int fd;

    fd = 0;
    if (t->x_factor)
        fd = create_temp_file(t->s);
    else
        fd = open(t->s, O_RDONLY);
    //gestione file temporaneo dell'heredoc
}

bool    do_open(t_cmd *cmd)
{
    t_flist *t;

    t = cmd->files;
    while(t)
    {
        if (cmd->in_fd != 0)
            close(cmd->in_fd);
        if (cmd->ou_fd != 0)
            close(cmd->ou_fd);
        if (t->amb_redi)
            return (ft_printf("minishell: %s: ambigous redirect\n", t->s), false);
        if (t->io_bool)
        {
            if (!open_out(t, cmd))
                return (false);
        }
        else
        {
            if (!open_in(t, cmd))
                return (false);
        }
        t = t->next;
    }
    return (true);
}

void    execute(t_cmd cmd_d, t_data *data)
{
    int pid;

    pid = 0;
    pid = fork();
    if (pid == -1)
        ft_printf("fork error");
    if (pid == 0)
    {
        if (!do_open(&cmd_d))
            return ;
    }
    /*apertura file con controlli di open, amb_redi e creazione file temp HERE_DOC
    controllo se built_in e mando a eseguire (sempre fare fork salvo in caso di exit e nessun pipe)
    se non e' built in otteniamo il path al comando(se non gia assoluto) e eventuale errore
    mandiamo alla esecuzione di execve*/
}

void    execution(t_data *data)
{
    int i;

    i = 0;
    while (i < data->cmd_count)
    {
        execute(data->cmd_arr[i], data);
        i++;
    }
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
        if (parsing(&data))
        {
            ft_printf("READY FOR EXECUTE\n");
            if (pipe(data.pipe) != -1)
                execution(&data);
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

//gestione $?, gestione signal in here_doc