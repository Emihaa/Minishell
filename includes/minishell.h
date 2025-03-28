/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:06:30 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/28 17:15:49 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


//@TODO: heredocuments have to be opened before any forking happens


#ifndef MINISHELL_H
# define MINISHELL_H

# define _GNU_SOURCE

#define ANTIKRISTA 666

// maybe change these to makefile link?
// @NOTE: would cause problems for vscode higlighting and autocomplete
#include "../libs/lt_alloc/includes/lt_alloc.h" 
#include "../libs/libft/includes/libft.h"

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

// maybe have all token types be negative except heredoc so that the type can be replace with an fd
// @question are these the only tokens needed?
typedef enum e_type
{
	PIPE			=	-'|',
	REDIRECT_IN		=	-'<',
	REDIRECT_OUT	=	-'>',
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
	t_arena		node_arena;
	t_arena		env_arena;
	t_arena		scratch_arena;
	char		**envp;
}	t_minishell;


// LUKA i have to handle | $ expansion | "" quote expansion | '' single quote expansion | white space removal
// EMILIA handles these


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
	} u_data;
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

// lexer stuff
t_token	get_next_token(t_lexer *lexer);
t_token *get_token_array(t_arena *arena, t_lexer *lexer); // not used currently @TODO: remove this
void print_tokens(t_lexer *lexer); // for debugging in the lexer

// tree stuff
t_node *parser(t_arena *arena, char *line);

// expand stuff
void expand(t_arena *arena, t_node *tree);

// out_expand stuff
void expand_out(t_arena *arena, t_node *tree);

// heredoc stuff
#define HEREDOC_TEMP_NAME "./heredoc_temp"
#define NAME_BASE_LEN sizeof(HEREDOC_TEMP_NAME) - 1
#define EOF_ERROR "minishell: warning: here-document at line %i \
delimited by end-of-file (wanted `%s')\n"
// int a = NAME_BASE_LEN; // delete
int heredoc(t_minishell *minishell, t_token *data);


// testing possible redirect stuff
#define WRITE	1
#define READ	0
void	store_write_fd(int write_fd, t_minishell *minishell);
void	store_read_fd(int read_fd, t_minishell *minishell);
void	apply_redirect(t_minishell *minishell);
void	reset_redirect(t_minishell *minishell);
int		redirect_out(char **file_name, t_minishell *m);
int		redirect_in(char **file_name, t_minishell *m);
int		redirect_append(char **file_name, t_minishell *m);

void	syscall_failure(t_minishell *m);
void	wait_for_sub_processes(t_minishell *minishell);


//environment stuff
// char	*find_env_var(const t_token *data, const uint32_t start, uint32_t *index, char **env);
char	*find_env_var(const char *str, const uint32_t str_len, uint32_t *index, char **env);

//general utils stuff
uint32_t set_quote_removed_string(char *string, t_token *data);
uint8_t	num_len(uint32_t num);
bool	is_space(char c);
t_minishell *get_minishell(t_minishell *m);
void error_exit(t_minishell *m, int exit_status); // probably not used

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

#endif