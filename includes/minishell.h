/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:06:30 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/14 17:32:19 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#define _GNU_SOURCE

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
#include <sys/stat.h>


// @question are these the only tokens needed?
typedef enum e_type
{
	REDIRECT_IN		=	'<',
	REDIRECT_OUT	=	'>',
	PIPE			=	'|',
	HERE_DOCUMENT	=	256, // <<
	REDIRECT_APPEND	=	257, // >>
	// COMMAND		=	258,
	// ARGUMENT		=	259, // @question with argv or not?
	WORD			=	260,	// generic word, could be command name or argument 
	END_OF_LINE		=	-1,		// first WORD before a pipe is the command and the following ones are arguments
	ERROR			=	-404,	// so that distinction can be made in the lexer if that would be better
}	t_type;

#define REDIRECT_IN_NAME	"<"
#define REDIRECT_OUT_NAME	">"
#define APPEND_NAME			">>"
#define HEREDOC_NAME		"<<"
#define PIPE_NAME			"|"
#define EOL_NAME			"newline"
#define WORD_NAME			"WORD"

#define ERROR_NAME			"ERROR"

// we should add
typedef struct s_minishell
{
	uint32_t	line_counter;
	uint32_t	command_count;
	uint32_t	heredoc_count;
	int			redir_fds[2];
	pid_t			*pids;
	int			exit_status;
	t_arena		node_arena;
	t_arena		scratch_arena;
	char		**envp;
}	t_minishell;


// LUKA i have to handle | $ expansion | "" quote expansion | '' single quote expansion | white space removal

// LUKA: maybe use this? idunno probably  not
// 
typedef union u_data 
{
	char	*filename;
	char	**command_argv;
}	t_data;

// LUKA: length based string
// I expect this will make creating tokens a bit easier
// and remove the need to allocate anything inside the lexer
// UNUSED 04.03.25 added to token struct
typedef struct s_token_string
{
	t_u64	length;		// total size of the string
	char	*pointer;	// where the string starts
}	t_token_string;

// struct for token information
// can be extender if necessary
// name should probably be created during parsing if the parser encounters a syntax error
typedef struct s_token
{
	t_type	type;	// the type of the token
	char	*string; 	// the string of the token | if applicable will be a filename a command name or an argument
	size_t	string_len;	// length of the string
	char	*name;	// added mostly for testing but might stay useful
}	t_token; //@TODO: re oder struct to reduce size;

// struct for any information the lexer might need
// currently only the line string and the index seem to be required
// parser will have to create this and pass it by reference to the lexer
typedef struct s_lexer
{
	char *line;
	t_u32 line_index;
}	t_lexer;


////// emilia nodetree stuff
typedef struct s_node
{
	t_token token; // t_token token

	struct s_node *root; // previous node this is connected to
	struct s_node *left; // struct on the under left
	struct s_node *right; // struct on the under right 
} t_node;

// lexer stuff
t_token	get_next_token(t_lexer *lexer);
t_token *get_token_array(t_arena *arena, t_lexer *lexer);
void print_tokens(t_lexer *lexer);

//heredoc stuff
#define HEREDOC_TEMP_NAME "./heredoc_temp"
#define NAME_BASE_LEN sizeof(HEREDOC_TEMP_NAME) - 1
// int a = NAME_BASE_LEN; // delete
int heredoc(t_minishell *minishell, t_token *data);


// testing possible redirect stuff
#define WRITE	1
#define READ	0
void store_redirects(int *in_fd, int *out_fd, t_minishell *minishell);
void apply_redirect(t_minishell *minishell);
void reset_redirect(t_minishell *minishell);

//general utils stuff
uint32_t get_quote_removed_string(char *string, t_token *data);
uint8_t	num_len(uint32_t num);

#endif