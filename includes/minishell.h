/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:06:30 by ltaalas           #+#    #+#             */
/*   Updated: 2025/02/27 17:40:43 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// @question are these the only tokens needed?
typedef enum e_token_type
{
	TOKEN_EXPANSION = '$',

	TOKEN_SINGLE_QUOTES = '\'',
	TOKEN_DOUBLE_QUOTES = '"',

	TOKEN_REDIRECT_LEFT = '<',
	TOKEN_REDIRECT_RIGHT = '>',
	
	TOKEN_PIPE = '|',

	TOKEN_HERE_DOCUMENT = 256,
	TOKEN_REDIRECT_APPEND = 257,

	TOKEN_

} t_token_type;

typedef struct s_token
{
	t_token_type type;
	char *string;
} t_token;

