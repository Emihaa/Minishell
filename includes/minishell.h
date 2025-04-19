/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:06:30 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/19 23:55:10 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


//@TODO: heredocuments have to be opened before any forking happens


#ifndef MINISHELL_H
# define MINISHELL_H

#define _GNU_SOURCE

#define ANTIKRISTA 666


// maybe change these to makefile link?
// @NOTE: would cause problems for vscode higlighting and autocomplete
#include "../libs/lt_arena/includes/lt_arena.h"
#include "../libs/libft/includes/libft.h"

#include <errno.h> // errno
#include <stddef.h> // size_t
#include <unistd.h>	//for write, pipe etc.
#include <sys/types.h>  // pid_t
#include <sys/wait.h> // wait, waitpid
#include <stdio.h>	// printf
#include <readline/readline.h> // readline
#include <readline/history.h> // add_history
#include <string.h>	// was for testing // might need for strerror or something else
#include <linux/limits.h>	// linux max length stuff
#include <stdbool.h>	// for bool data type
#include <fcntl.h>		// open
#include <sys/stat.h>	// lstat !!! might not be used WATCH OUTTTTTAMSL:MF:ALMSG:LMA
#include <signal.h>     // signal handling


// find proper place for these thingies
char *get_env_var_value(char *key, uint32_t key_len);

extern volatile sig_atomic_t g_int;

// maybe have all token types be negative except heredoc so that the type can be replace with an fd
// @question are these the only tokens needed?
typedef enum e_type
{
	PIPE			=	-'|',
	REDIRECT_IN		=	-'<',
	REDIRECT_OUT	=	-'>',
	REDIRECT_AMBI	=	-41, // if redirect is ambigious redirect
	HERE_DOCUMENT	=	256, // <<
	REDIRECT_APPEND	=	-257, // >>
	// COMMAND		=	258,
	// ARGUMENT		=	259,	 // @question with argv or not?
	WORD			=	-260,	// generic word, could be command name or argument
	END_OF_LINE		=	0,		// first WORD before a pipe is the command and the following ones are arguments
	ERROR			=	-404,	// so that distinction can be made in the lexer if that would be better
}	t_type;

// we should add globally needed stuff here
// @TODO: restucture to reduce memory footprint
typedef struct s_minishell
{
	int			istty;
	uint32_t	line_counter;
	uint32_t	command_count;
	uint32_t	heredoc_count;
	int			*heredoc_fds;
	int			redir_fds[2];
	int			pipe[2];
	int8_t		pipe_side;
	pid_t			*pids;  // maybe needed
	pid_t			last_pid;
	int			exit_status;
	char		*line;
	t_arena		*node_arena;
	char		**envp;
	int			envp_size;
	int			env_capacity;
}	t_minishell;


// LUKA: maybe use this? idunno probably  not
//
typedef union u_data
{
	char	*string;
	char	**command_argv;
}	t_data;

// LUKA: length based string
// I expect this will make creating tokens a bit easier
// and remove the need to allocate anything inside the lexer
// UNUSED 04.03.25 added to token struct
typedef struct s_token_string
{
	uint64_t	length;		// total size of the string
	char	*pointer;	// where the string starts
}	t_token_string;

// struct for token information
// can be extender if necessary
// name should probably be created during parsing if the parser encounters a syntax error
typedef struct s_token
{
	t_type		type;	// the type of the toke
	uint32_t	string_len;	// length of the string // changed from size_t to uint32_t. 15.03.2025
	union
	{
		char *string;
		char **argv; 	// the string of the token | if applicable will be a filename a command name or an argument
	};
}	t_token;

// struct for any information the lexer might need
// currently only the line string and the index seem to be required
// parser will have to create this and pass it by reference to the lexer
typedef struct s_lexer
{
	char *line;
	uint32_t line_index;
}	t_lexer;

////// emilia nodetree stuff
typedef struct s_node
{
	t_token token; // t_token token

	struct s_node *root; // previous node this is connected to
	struct s_node *left; // struct on the under left
	struct s_node *right; // struct on the under right
} t_node;

// minishell main stuff
void minishell_cleanup(t_minishell *minishell);
void signal_handler(int signal);

// lexer stuff
t_token	get_next_token(t_lexer *lexer);

// tree stuff
t_node	*parser(t_arena *arena, t_minishell *m, char *line);

// tree_utils stuff
bool heredoc_limit(t_token *token, uint32_t *heredoc_count);
void *syntax_error(t_token *token, t_lexer *lexer);
t_node *find_head_root(t_node *node);
void print_token_type(t_type *token_type);

// tree_nodes stuff
t_node *create_node(t_node *root, t_token *token, t_arena *arena);
t_node *insert_middle(t_node *root, t_node *node, t_token *token, t_arena *arena);
t_node *insert_below(t_node *root, t_node *node, t_token *token, t_arena *arena);
t_node *insert_bottom(t_node *node, t_token *token, t_arena *arena);
t_node *insert_top(t_node *node, t_token *token, t_arena *arena);

// expand stuff
typedef struct s_expand_vars
{
	uint32_t i;
	uint32_t len;
	char quote;
	bool had_quote;
	char *env_var;
}	t_expand_vars;

// Argument vector that holds the argument data inside it.
// Also holds the info of memory capacity and currently used size
typedef struct s_arg_vec
{
	size_t size;
	size_t capacity;
	char	**data;
} t_arg_vec;

// Argument string info, lenght & index.
// Boolean for argument.exist for NULL terminated argument(?)
typedef struct s_arg
{
	char	*data_str;
	uint32_t data_len;
	uint32_t i;
	bool	exist;
} t_arg;

# define ARGV_DEFAULT_SIZE 32
void	init_argv(t_arena *arena, t_arg_vec *argv, size_t cap);
void	init_arg(t_token *data, t_arg *arg_vars);
int		realloc_argv(t_arena *arena, t_arg_vec *argv);
int		is_quote_or_var(char c);

// argv stuff
char	**create_argv(t_arena *arena, t_node *node);

int	expand(t_arena *arena, t_minishell *m, t_node *tree);
int handle_variable(t_arena *arena, t_string *str, t_arg *arg, t_arg *leftover);
void	handle_quote(t_arena *arena, t_arg *arg, t_string *str);

// expand_redirect stuff
void expand_redirect(t_arena *arena, t_node *node);
int expansion_stuffs(t_node *node, t_expand_vars *v, char *str);

int	quote_check(t_node *node, t_expand_vars *v);
int handle_var(t_arena *arena, t_string *str, t_arg *arg, t_arg *leftover);

char	*find_env_var(const char *str, const uint32_t str_len, uint32_t *index, char **env);
uint32_t	set_quote_removed_string(char *string, t_token *data);

//expand_utils stuff
void	copy_exit_code(t_arena *arena, t_arg *arg, t_string *str);

// expand_in_quotes_stuff
void	handle_quote(t_arena *arena, t_arg *arg, t_string *str);
void	handle_variable_in_quotes(t_arena *arena, t_arg *arg, t_string *str);

void	copy_in_single_quote(t_arena *arena, t_arg *arg, t_string *str);

void	copy_in_double_quote(t_arena *arena, t_arg *arg, t_string *str);

void	copy_full_env_var(t_arena *arena, t_arg *arg, t_string *str);



void	copy_until_quote_or_var(t_arena *arena, t_arg *arg, t_string *str);
void	copy_full_env_var(t_arena *arena, t_arg *arg, t_string *str);
void	copy_exit_code(t_arena *arena, t_arg *arg, t_string *str);
void	expand_variable_in_quotes(t_arena *arena, t_arg *arg, t_string *str);

bool	is_valid_var_start(char c);
int		small_itoa(t_expand_vars *v, char *str);

// heredoc stuff
# define HEREDOC_TEMP_NAME "./heredoc_temp"
# define NAME_BASE_LEN sizeof(HEREDOC_TEMP_NAME) - 1
# define EOF_ERROR "minishell: warning: here-document at line %i \
delimited by end-of-file (wanted `%s')\n"
// int a = NAME_BASE_LEN; // delete
int heredoc(t_arena *arena, t_minishell *minishell, t_token *data);

//	heredoc_utils stuff
char	*create_temp_file_name(uint32_t heredoc_num);
int		create_heredoc_fds(int fds[2]);
int		heredoc_event_hook(void);
void	close_heredocs(t_minishell *m); // in wrong file atm
void	print_eof_error(t_minishell *m, char *delimiter);
int		heredoc_read(t_minishell *minishell, char **line, char *delimiter);
// testing possible redirect stuff
#define WRITE	1
#define READ	0
void	store_write_fd(int write_fd, t_minishell *minishell);
void	store_read_fd(int read_fd, t_minishell *minishell);
void	apply_redirect(t_minishell *minishell);
void	reset_redirect(t_minishell *minishell);
int		redirect_out(char *file_name, t_minishell *m);
int		redirect_in(char *file_name, t_minishell *m);
int		redirect_append(char *file_name, t_minishell *m);
int		redirect_ambi(char *file_name);

void	wait_for_sub_processes(t_minishell *minishell);


// export stuff
void builtin_export(t_minishell *m, int argc, char **argv);
int char_pos(char *s, char c);
void print_export_env(char **env);
void swap_pntrs(char **p1, char **p2);
int is_sorted(char **pntr_array, int size);
void env_sort(char **pntr_array, int size);
int find_slot(t_minishell *m);
int check_match(t_minishell *m, char *s);
int spelling_check(t_minishell *m, char *s);
int	export_add(t_minishell *m, char *s);

// unset stuff
void builtin_unset(t_minishell *m, int argc, char **argv);

// cd stuff
void builtin_cd(t_minishell *m, int argc, char **argv);

// envi stuff
void	builtin_env(char **envp);
char **create_env(t_minishell *m, char **envp);
int create_new_env(t_minishell *m , char **envp);
void print_export(t_minishell *m);
// char	*find_env_var(const t_token *data, const uint32_t start, uint32_t *index, char **env);
int	get_env_key_index(char *key, uint32_t key_len, char **envp);
uint32_t	get_env_key_len(char *src, uint32_t src_len);
char *get_env_var_value(char *key, uint32_t key_len);

//general utils stuff
uint8_t		num_len(uint32_t num);
bool		is_space(char c);
uint32_t	eat_space(char *str);
bool		is_quote(char c);
t_minishell *get_minishell(t_minishell *m);

// arena_utils stuff
void *xarena_alloc(t_arena *arena, uint64_t size);
void *xarena_alloc_no_zero(t_arena *arena, uint64_t size);
t_arena *xarena_new(uint64_t cap);

// ARENA strings

int	append_to_string(t_arena *a, t_string *str, char *src, size_t src_len);
int	string_find_new_memory(t_arena *a, t_string *str, size_t new_size);
int terminate_and_commit_string(t_arena *a, t_string *str);


//builtin stuff
typedef enum e_builtin
{
	BUILTIN_FALSE = 0,
	BUILTIN_EXIT,
	BUILTIN_ECHO,
	BUILTIN_CD,
	BUILTIN_PWD,
	BUILTIN_ENV,
	BUILTIN_UNSET,
	BUILTIN_EXPORT,
} t_builtin;

t_builtin check_for_builtin(char *command);
int	execute_builtin(t_minishell *m, char **argv, t_builtin command);

void builtin_exit(t_minishell *m, char **argv);
void builtin_echo(char *argv[], int fd);
void builtin_pwd(t_minishell *m, int fd);


// execute stuff
pid_t	execute_subprocess(t_arena *arena, t_minishell *m, char **argv, t_builtin builtin);
int	execute_command(t_arena *arena, t_minishell *m, char **argv, int status);

void close_pipe(t_minishell *m);
void execve_failure(t_minishell *m, char *cmd);
void command_not_found(t_minishell *m, char *cmd);


//error stuff
void	error_exit(t_minishell *m, int exit_status);
void	syscall_failure(t_minishell *m);


// write_functions stuff
int	write_bytes(int fd, char *str, size_t bytes_to_write);
int	put_str(int fd, char *str);
int	put_str_nl(int fd, char *str);
int	put_char(int fd, char c);


#endif
