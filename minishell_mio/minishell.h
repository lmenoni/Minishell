/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 16:21:28 by lmenoni           #+#    #+#             */
/*   Updated: 2025/05/03 16:49:44 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include "my_libft/libft.h"
#include <stdio.h>          // printf, perror, getcwd, fopen, fclose, etc.
#include <stdlib.h>         // malloc, free, exit, getenv, etc.
#include <unistd.h>         // access, open, read, close, fork, execve, dup, dup2, pipe, isatty, etc.
#include <sys/types.h>      // opendir, readdir, closedir, stat, lstat, fstat, wait, waitpid, etc.
#include <sys/stat.h>       // stat, lstat, fstat, mkdir, etc.
#include <fcntl.h>          // open, fcntl, etc.
#include <dirent.h>         // opendir, readdir, closedir
#include <string.h>         // strerror, etc.
#include <signal.h>         // signal, sigaction, sigemptyset, sigaddset, kill
#include <sys/wait.h>       // wait, waitpid, wait3, wait4
#include <termios.h>        // tcsetattr, tcgetattr
#include <term.h>           // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
#include <sys/ioctl.h>      // ioctl
#include <readline/readline.h>  // readline, rl_clear_history, etc.
#include <readline/history.h>   // add_history, etc.
#include <errno.h>          // errno, perror, strerror

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define RESET "\033[0m"

typedef struct  s_data
{
    char *input;
    char **input_array;
}               t_data;

void	error_handle(int error, int exit_type);
void	ft_free(char **mat);
char	**get_path(char **env, int file);
char	*find_cmd_path(char *cmd, char **path_dirs);
void	exec(char **argv, char **env, char **path, int cmd_index);
void	heredoc(char **argv);
int		open_file(char *file, int flags);
#endif