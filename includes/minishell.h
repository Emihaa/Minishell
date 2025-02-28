/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:06:30 by ltaalas           #+#    #+#             */
/*   Updated: 2025/02/28 23:23:33 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "../libs/lt_alloc/includes/lt_alloc.h" // maybe change to makefile link?
#include "../libs/libft/includes/libft.h"

// @question are these the only tokens needed?
typedef enum
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

typedef struct s_token
{
	t_token_type	type;
	char			*string;
}	t_token;

typedef struct s_lexer
{
	char *line;
	t_u32 line_index;
}	t_lexer;



#endif