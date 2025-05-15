/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <lmenoni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 16:21:28 by lmenoni           #+#    #+#             */
/*   Updated: 2025/05/15 13:36:48 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include "my_libft/libft.h"
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
    EXPAND,
    ARGUMENT,
    STRING,
    PIPE,
    OPERATOR,
    REDI_IN,
    REDI_OUT,
    HERE_DOC,
    APPEND,
}       tok_type;

typedef struct  s_env
{
    char			*e;
    struct s_env	*next;
	struct s_env	*prev;
}               t_env;

typedef struct  s_flist
{
    char            *s;
    bool            x_factor;
    // bool            io_bool;
    struct s_flist  *next;
}                   t_flist;

typedef struct  s_token
{
    char			*s;
    tok_type		type;
    struct s_token	*next;
	struct s_token	*prev;
}               t_token;

typedef struct s_cmd
{
    t_flist	*input;
    t_flist	*output;
    char	**args;
    bool    pipe_out;
    bool    pipe_in;
}			t_cmd;

typedef struct  s_data
{
    //char        *curr_path;
    //char        *old_path;
    //char        *home_path;
    t_env        *env_data;
    char        *input;
    t_cmd       *cmd_arr;
    t_token     *token;
    t_token     *last_token;
    int         cmd_count;
}               t_data;

//cmd_array.c
void    make_cmd_array(t_data *data);
t_cmd    *add_cmd_to_arr(t_data *data);
void    add_files_to_arr(t_cmd *cmd, t_token **head, t_data *data);

//cmd_array_utils.c
void    get_args(t_cmd *cmd, t_token **curr);
void    add_file_node(t_flist **head, char *s, bool x);
void    remove_two(t_token **head, t_token **t, t_data *data);

//tokenization.c
void    define_token_add(t_data *data, char *s, int *idx);
void    tokenize_input(t_data *data);

//add_token.c
void    add_string(t_data *data, char *s, int *idx, char c);
void    add_pipe(t_data *data, char *s, int *idx);
void    add_redirect(t_data *data, char *s, int *idx, char c);
void    add_operator(t_data *data, char *s, int *idx);
void    add_argument(t_data *data, char *s, int *idx);

//token_utils.c
void    print_tokens(t_token *token);
void    add_token(t_token **first, char *s, tok_type type, t_data *data);
t_token *token_new(char *content, tok_type type);
tok_type    which_type(char c, int len);
void    handle_quotes(char *quote, bool *in_quotes, char c, int *i);

//free_mem.c
void    free_token(t_token *token);
void    free_flist(t_flist *flist);
void    free_cmd_array(t_data *data);

//utils.c
void    skip_spaces(char *s, int *i);
int is_space(char c);
void    print_files(t_flist *list);
void    print_cmd_array(t_data *data);

#endif

// in caso di "   "qualcosa fare in modo che il token sia ""    "qualcosa"
// cambiare metodo per la lista di file(una sola con un bbool per riconoscerli)
// tutti gli argomenti che hanno "" o '' togliendo gli apici e espandere $ se tra ""
//prima espandere $ dentro le virgolette poi quelli fuori e dividi con gli spazi i nuovi token
// i token $ espansi creando token divisi da spazi