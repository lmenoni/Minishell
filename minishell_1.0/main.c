/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 16:21:16 by lmenoni           #+#    #+#             */
/*   Updated: 2025/06/09 18:28:56 by igilani          ###   ########.fr       */
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
    // print_tokens(data->token);
    make_cmd_array(data);
    // print_cmd_array(data);
    return (true);
}

// bool    open_out(t_flist *t, t_cmd *cmd)
// {
//     int fd;

//     fd = 0;
//     if (t->x_factor)
//         fd = open(t->s, O_WRONLY | O_CREAT | O_APPEND, 0644);
//     else
//         fd = open(t->s, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//     if (fd < 0)
//         return (ft_printf("minishell: %s: ", t->s), perror(""), false);
//     cmd->ou_fd = fd;
//     return (true);
// }

// bool    open_in(t_flist *t, t_cmd *cmd)
// {
//     int fd;

//     fd = 0;
//     if (t->x_factor)
//         fd = create_temp_file(t->s);
//     else
//         fd = open(t->s, O_RDONLY);
//     //gestione file temporaneo dell'heredoc
// }

// bool    do_open(t_cmd *cmd)
// {
//     t_flist *t;

//     t = cmd->files;
//     while(t)
//     {
//         if (cmd->in_fd != 0)
//             close(cmd->in_fd);
//         if (cmd->ou_fd != 0)
//             close(cmd->ou_fd);
//         if (t->amb_redi)
//             return (ft_printf("minishell: %s: ambigous redirect\n", t->s), false);
//         if (t->io_bool)
//         {
//             if (!open_out(t, cmd))
//                 return (false);
//         }
//         else
//         {
//             if (!open_in(t, cmd))
//                 return (false);
//         }
//         t = t->next;
//     }
//     return (true);
// }

// void    execute(t_cmd cmd_d, t_data *data)
// {
//     int pid;

//     pid = 0;
//     pid = fork();
//     if (pid == -1)
//         ft_printf("fork error");
//     if (pid == 0)
//     {
//         if (!do_open(&cmd_d))
//             return ;
//     }
//     /*apertura file con controlli di open, amb_redi e creazione file temp HERE_DOC
//     controllo se built_in e mando a eseguire (sempre fare fork salvo in caso di exit e nessun pipe)
//     se non e' built in otteniamo il path al comando(se non gia assoluto) e eventuale errore
//     mandiamo alla esecuzione di execve*/
// }

void define_input(t_data *data, t_cmd *cmd)
{
	if (!ft_strncmp(cmd->args[0], "echo", 4))
		echo(cmd->args);
    else if (!ft_strncmp(cmd->args[0], "cd", 2))
		cd(data, cmd, cmd->args);
	else if (!ft_strncmp(cmd->args[0], "pwd", 3))
		pwd(data);
	else if (!ft_strncmp(cmd->args[0], "env", 3))
		env(data, cmd->args);
	else if (!ft_strncmp(cmd->args[0], "unset", 5))
		unset(data, cmd->args);
	else if (!ft_strncmp(cmd->args[0], "export", 6))
		export(data, cmd->args);
    else if (!ft_strncmp(cmd->args[0], "exit", 4))
        exit_shell(data, cmd);
}

// void    execution(t_data *data, t_cmd cmd)
// {
//     int i;

//     i = 0;
//     while (i < data->cmd_count)
//     {
//         execute(data->cmd_arr[i], data);
//         i++;
//     }
// }

int main(int ac, char **av, char **e)
{
    t_data  data;
    (void)ac;
    (void)av;
    data = (t_data){0};
    data.env_data = init_env(e, &data);
    if (!check_env(&data, "OLDPWD"))
		add_env(&data, "OLDPWD");
    //print_env(data.env_data);
    while (1)
    {
        reset_data(&data);
        data.input = readline(CYAN"minishell"RESET YELLOW">"RESET);
        if (!data.input)
            break ;
        if (parsing(&data))
        {
            define_input(&data, &data.cmd_arr[0]);
            // ft_printf("READY FOR EXECUTE\n");
            // if (pipe(data.pipe) != -1)
            // execution(&data, data.cmd_arr[0]);
        }
        add_history(data.input);
        free(data.input);
        free_token(data.token);
        free_cmd_array(&data);
    }
    free_env(data.env_data);
    free(data.current_path);
    rl_clear_history();
    return (0);
}

//gestione $?, gestione signal in here_doc