/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:06:30 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/01 23:40:55 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "../libs/lt_alloc/includes/lt_alloc.h" // maybe change to makefile link?
#include "../libs/libft/includes/libft.h"

#include <stdbool.h>

// @question are these the only tokens needed?
typedef enum e_token_type
{
	REDIRECT_IN		=	'<',
	REDIRECT_OUT	=	'>',
	PIPE			=	'|',
	HERE_DOCUMENT	=	256, // <<
	REDIRECT_APPEND	=	257, // >>
	COMMAND			=	258, // with argv or not
	END_OF_LINE		=	-1,
}	t_token_type;


// LUKA i have to handle | $ expansion | "" quote expansion | '' single quote expansion | white space removal

// LUKA: maybe use this? idunno propably  not
// 
typedef union u_data 
{
	char	*filename;
	char	**command_argv;
};

// LUKA: length based string
// I expect this will make creating tokens a bit easier
// and remove the need to allocate anything inside the lexer
typedef struct s_token_string
{
	t_u64	length;
	char	*pointer;
}	t_token_string;

typedef struct s_token
{
	t_token_type	type;
	t_token_string	string;
}	t_token;

typedef struct s_lexer
{
	char *line;
	t_u32 line_index;
}	t_lexer;



#endif