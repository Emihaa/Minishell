/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:06:30 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/03 20:43:30 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "../libs/lt_alloc/includes/lt_alloc.h" // maybe change to makefile link?
#include "../libs/libft/includes/libft.h"

#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <linux/limits.h>
#include <stdbool.h>

// @question are these the only tokens needed?
typedef enum e_token_type
{
	REDIRECT_IN		=	'<',
	REDIRECT_OUT	=	'>',
	PIPE			=	'|',
	HERE_DOCUMENT	=	256, // <<
	REDIRECT_APPEND	=	257, // >>
	// COMMAND			=	258,
	// ARGUMENT		=	259, // @question with argv or not?
	WORD			=	260,	// generic word, could be command name or argument
	END_OF_LINE		=	-1,
	ERROR			=	-204,
}	t_token_type;

#define REDIRECT_IN_NAME	"<"
#define REDIRECT_OUT_NAME	">"
#define APPEND_NAME			">>"
#define HEREDOC_NAME		"<<"
#define PIPE_NAME			"|"
#define EOL_NAME			"newline"
#define WORD_NAME			"WORD"

#define ERROR_NAME			"ERROR"

// LUKA i have to handle | $ expansion | "" quote expansion | '' single quote expansion | white space removal

// LUKA: maybe use this? idunno propably  not
// 
typedef union u_data 
{
	char	*filename;
	char	**command_argv;
}	t_data;

// LUKA: length based string
// I expect this will make creating tokens a bit easier
// and remove the need to allocate anything inside the lexer
typedef struct s_token_string
{
	t_u64	length;		// total size of the string
	char	*pointer;	// where the string starts
}	t_token_string;

typedef struct s_token
{
	t_token_type	type;	// the type of the token
	t_token_string	string;	// the string of the token | if applicable will be a filename a command name or an argument
	char			*name;	// added mostly for testing but might stay usefull
}	t_token;

typedef struct s_lexer
{
	char *line;
	t_u32 line_index;
}	t_lexer;



#endif