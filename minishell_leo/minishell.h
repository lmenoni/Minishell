/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilani <igilani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 16:21:28 by lmenoni           #+#    #+#             */
/*   Updated: 2025/05/23 15:34:13 by igilani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include "my_libft/libft.h"
#include <stdbool.h>         // bool
#include <sys/types.h>      // opendir, readdir, closedir, stat, lstat, fstat, wait, waitpid, etc.
#include <sys/stat.h>       // stat, lstat, fstat, mkdir, etc.
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

typedef enum
{
    COMMAND,
    ARGUMENT,
    STRING,
    PIPE,
    OPERATOR,
    REDIRECT
}       tok_type;

typedef struct  s_env
{
    char			*e;
    char            *x;
    struct s_env	*next;
}               t_env;

typedef struct  s_token
{
    char			*s;
    tok_type		type;
    struct s_token	*next;
	struct s_token	*prev;
}               t_token;

typedef struct  s_data
{
    char        *input;
    char       **input_array;
    char        *current_path;
    char        *old_path;
    char        *home_path;
    t_token     *token;
    t_token     *last_token;
    t_env       *env_data;
}               t_data;

void    tokenize_input(t_data *data);

//add_token.c
void    add_string(t_data *data, char *s, int *idx);
void    add_pipe(t_data *data, char *s, int *idx);
void    add_redirect(t_data *data, char *s, int *idx);
void    add_operator(t_data *data, char *s, int *idx);
void    add_argument(t_data *data, char *s, int *idx);

//token_utils.c
void    print_tokens(t_token *token);
void    add_token(t_token **first, char *s, tok_type type, t_data *data);
t_token *token_new(char *content, tok_type type);

//free_mem.c
void    free_token(t_token *token);

//utils.c
void    skip_spaces(char *s, int *i);
int is_space(char c);
void print_error(char *s);

//pipex
void	error_handle(int error, int exit_type);
void	ft_free(char **mat);
char	**get_path(char **env, int file);
char	*find_cmd_path(char *cmd, char **path_dirs);
void	exec(char **argv, char **env, char **path, int cmd_index);
void	heredoc(char **argv);
int		open_file(char *file, int flags);

// env
t_env   *init_env(char **e, t_data *data);
t_env   *new_env_node(char *s);
char *check_env(t_data *data, char *var);
void update_env(t_data *data, char *var, char *str);
void delete_env(t_data *data, char *var);
void add_env(t_data *data, char *var);

// export
void export (t_data *data, char **args);

// builtin
void    echo(t_data *data);
void cd(t_data *data);
void unset(t_data *data, char **var);
void env(t_data *data, char **input_array);
void pwd();

void    print_cd(t_data *data);
#endif