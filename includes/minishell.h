/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:06:30 by ltaalas           #+#    #+#             */
/*   Updated: 2025/02/28 18:41:37 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// @question are these the only tokens needed?
typedef enum e_token_type
{
	EXPANSION 		=	'$',
	SINGLE_QUOTES 	=	'\'',
	DOUBLE_QUOTES	=	'"',
	REDIRECT_IN		=	'<',
	REDIRECT_OUT 	=	'>',
	PIPE 			=	'|',
	HERE_DOCUMENT	= 	256,
	REDIRECT_APPEND =	257,
	COMMAND			= 	258,

} t_token_type;

typedef struct s_token
{
	t_token_type type;
	char *string;
} t_token;

