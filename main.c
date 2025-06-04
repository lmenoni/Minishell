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
    //print_cmd_array(data);
    return (true);
}

bool    no_more_input(t_flist *t)
{
    t_flist *curr;

    curr = t->next;
    while (curr)
    {
        if (!(curr->io_bool))
            return (false);
        curr = curr->next;
    }
    return (true);
}

int     create_temp_file(char *content, t_data *data)
{
    char    *name;
    char    *path;
    int     fd;

    fd = 0;
    name = ft_itoa(data->cmd_count);
    path = ft_strjoin(".tmp_hd_n", name);
    fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0700);
    if (fd < 0)
        return (free(name), free(path), fd);
    ft_putstr_fd(content, fd);
    close(fd);
    fd = open(path, O_RDONLY);
    unlink(path);
    return (free(name), free(path), fd);
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

bool    open_in(t_flist *t, t_cmd *cmd, t_data *data)
{
    int fd;

    fd = 0;
    if (t->x_factor && no_more_input(t))
        fd = create_temp_file(t->s, data);
    else
        fd = open(t->s, O_RDONLY);
    if (fd < 0)
        return (ft_printf("minishell: %s: ", t->s), perror(""), false);
    cmd->in_fd = fd;
    return (true);
}

bool    do_open(t_cmd *cmd, t_data *data)
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
            if (!open_in(t, cmd, data))
                return (false);
        }
        t = t->next;
    }
    return (true);
}

void    set_pipe(t_cmd *cmd, t_data *data)
{
    if (data->cmd_name > 0)
        cmd->in_fd = data->pipe[data->cmd_name - 1][0];
    if (data->cmd_name < (data->cmd_count - 1))
        cmd->ou_fd = data->pipe[data->cmd_name][1];
}

char    *get_path(char *cmd, t_data *data)
{
    char    **fpath;
    char    *t;
    char    *r;
    int     i;

    i = 0;
    if (cmd[0] == '/')
        return (ft_strdup(cmd));
    fpath = ft_split(check_env(data, "PATH="), ':');
    if (!fpath)
        return (NULL);
    t = ft_strjoin("/", cmd);
    while(fpath[i])
    {
        r = ft_strjoin(fpath[i], t);
		if (access(r, F_OK) == 0 && access(r, X_OK) == 0)
			return (ft_freemat((void **)fpath, (ft_matlen(fpath) - 1))
                , free(t), r);
		free(r);
		i++;
    }
    return (ft_freemat((void **)fpath, (ft_matlen(fpath) - 1)), free(t), NULL);
}

void    execute(t_cmd cmd, t_data *data)
{
    int pid;

    pid = 0;
    // if (ft_strcmp(cmd_d.args[0], "exit") == 0 && !cmd_d.pipe_in && !cmd_d.pipe_out)
    //     handle_exit();
    pid = fork();
    if (pid == -1)
        ft_printf("fork error\n");
    if (pid == 0)
    {
        if (data->pipe)
            set_pipe(&cmd, data);
        if (!do_open(&cmd, data))
            return ; //deve liberare la memoria e fare exit non return
        if (cmd.in_fd != 0)
            dup2(cmd.in_fd, STDIN_FILENO);
        if (cmd.ou_fd != 0)
            dup2(cmd.ou_fd, STDOUT_FILENO);
        if (!define_input(data, &cmd))
        {
            cmd.path = get_path(cmd.args[0], data);
            // do_execve()
        }
        exit (0);
    }
    /*controllo se built_in e mando a eseguire (sempre fare fork salvo in caso di exit e nessun pipe)
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
        data->cmd_name++;
        i++;
    }
    while (!wait(NULL))
        ;
}

void    create_pipe_arr(t_data *data)
{
    int i;

    i = 0;
    if (data->cmd_count > 1)
    {
        data->pipe = malloc((data->cmd_count - 1) * sizeof(int *));
        while (i < (data->cmd_count - 1))
        {
            data->pipe[i] = malloc(2 * sizeof(int));
            pipe(data->pipe[i]);
            i++;   
        }
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
    rl_clear_history();
    return (0);
}

//gestione $?, gestione signal in here_doc
//gestione fallimento pipe array, controllare open funzionino correttamente
//, fare execve e funzioni per liberare tutta la memoria nei figli(va stritta quella del pipearr)