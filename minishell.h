/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmenoni <lmenoni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 16:21:28 by lmenoni           #+#    #+#             */
/*   Updated: 2025/05/21 17:59:15 by lmenoni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include "my_libft/libft.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <termios.h>
#include <term.h>
#include <sys/ioctl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <errno.h>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define RESET "\033[0m"

typedef enum
{
    ARGUMENT,
    DOLLAR,
    PIPE,
    REDI_IN,
    REDI_OUT,
    HERE_DOC,
    APPEND,
}       tok_type;

typedef struct  s_env
{
    char			*e;
    struct s_env	*next;
}               t_env;

typedef struct  s_flist
{
    char            *s;
    bool            x_factor;
    bool            io_bool;
    struct s_flist  *next;
}                   t_flist;

typedef struct  s_token
{
    char			*s;
    tok_type		type;
    struct s_token  *attach;
    struct s_token	*next;
	struct s_token	*prev;
}               t_token;

typedef struct s_cmd
{
    t_flist	*files;
    char	**args;
    bool    pipe_out;
    bool    pipe_in;
}			t_cmd;

typedef struct  s_data
{
    char        *curr_path;
    char        *old_path;
    char        *home_path;
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
void    add_file_node(t_flist **head, char *s, bool x, bool io);
void    remove_two(t_token **head, t_token **t, t_data *data);

//tokenization.c
void    tokenize_input(t_data *data);
void    add_pipe(t_data *data, int *idx);
void    add_redirect(t_data *data, char *s, int *idx, char c);
void    add_argument(t_data *data, char *s, int *idx, bool attach);
void	add_dollar(t_data *data, char *s, int *idx, bool attach);
int     argument_len(char *s, t_token *last_token);

//token_utils.c
void    add_token(char *s, tok_type type, t_data *data, bool attach);
t_token *token_new(char *content, tok_type type);
tok_type    which_type(char c, int len);

//free_mem.c
void    free_token(t_token *token);
void    free_flist(t_flist *flist);
void    free_cmd_array(t_data *data);
void    free_env(t_env *node);

//utils.c
void    skip_spaces(char *s, int *i);
int is_space(char c);
int parse_syntax_errors(t_token *token);
void    reset_data(t_data *data);
int	is_quoted(char *s, char *to_check);

//env.c
t_env   *init_env(char **e, t_data *data);
t_env   *new_env_node(char *s);

//print.c
void    print_tokens(t_token *token);
void    print_files(t_flist *list);
void    print_cmd_array(t_data *data);
void    print_env(t_env *node);

//here_doc.c
void    do_here_doc(t_token *tok, t_data *data);
char    *get_lines(char *s);
char    *get_limiter(char *s);
void    fill_limiter(char **r, char *s);
int limiter_len(char *s);
bool    is_limiter_quoted(char *s);

//expansion.c
char	*get_expanded_var(char *s, int *idx, t_data *data);
void	while_not_var(char *s, char *t, int *i, bool expand);
char *check_env(t_data *data, char *var);
int	is_quoted(char *s, char *to_check);
char *expand_dollar(char *s, t_data *data, bool expand);

#endif

// cambiare metodo per la lista di file(una sola con un bbool per riconoscerli)
// tutti gli argomenti che hanno "" o '' togliendo gli apici e espandere $ se tra ""
//prima espandere $ dentro le virgolette poi quelli fuori e dividi con gli spazi i nuovi token
// i token $ espansi creando token divisi da spazi