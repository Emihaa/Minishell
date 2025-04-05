/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 22:33:07 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/03 19:25:14 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 
// change to be a just "minishell.h"
// after making sure makefile linking works properly

// does the current character delimit a word
static inline
bool	is_delimiter(char c)
{
	if (c == '|')
		return (true);
	if (c == '<')
		return (true);
	if (c == '>')
		return (true);
	if (c == '\0')
		return (true);
	if (is_space(c))
		return (true);
	return (false);
}

// moves in the line until finds a matching quote or the EOL 
static inline
int	match_quote(t_lexer *lexer, char quote, int len)
{
	char	c;

	if (quote == '\'' || quote == '"')
	{
		len += 1;
		while (1)
		{
			c = (lexer->line[lexer->line_index + len]);
			if (c == quote || c == '\0')
				break ;
			len += 1;
		}
	}
	return (len);
}

// if we hit a quote len will for sure be at least 1
// so might break the len check

// creates a token matching the input parameters
// for tokens that can have a string
// @TODO: rename
static
t_token	tokenize_stuffs(t_lexer *lexer, t_type type, int to_skip)
{
	t_token	token;
	char	c;
	int		len;

	len = 0;
	lexer->line_index += to_skip;
	while (is_space(lexer->line[lexer->line_index]))
		lexer->line_index += 1;
	while (1)
	{
		c = lexer->line[lexer->line_index + len];
		len = match_quote(lexer, c, len);
		if (is_delimiter(c))
			break ;
		len += 1;
	}
	token = (t_token)
	{
		.type = type,
		.string_len = len,
		.u_data.string = &lexer->line[lexer->line_index],
	};
	lexer->line_index += len;
	return (token);
}

// just returns a token with correct parameters for a pipe
static inline
t_token	tokenize_pipe(t_lexer *lexer)
{
	const t_token	token = {
		.type = PIPE,
		.u_data.string = &lexer->line[lexer->line_index],
		.string_len = 1,
	};

	lexer->line_index += 1;
	return (token);
}

// skips whitespace between tokens and finds the next token type to return
// @TODO:	find out which character can be valid parts of a word
// 			currently anything other than a delimiter character counts as a word
//			which doesn't seem correct
t_token	get_next_token(t_lexer *lexer)
{	
	while (is_space(lexer->line[lexer->line_index]))
		lexer->line_index += 1;
	if (ft_strncmp(&lexer->line[lexer->line_index], ">>", 2) == 0)
		return (tokenize_stuffs(lexer, REDIRECT_APPEND, 2));
	if (ft_strncmp(&lexer->line[lexer->line_index], "<<", 2) == 0)
		return (tokenize_stuffs(lexer, HERE_DOCUMENT, 2));
	if (lexer->line[lexer->line_index] == '|')
		return (tokenize_pipe(lexer));
	if (lexer->line[lexer->line_index] == '<')
		return (tokenize_stuffs(lexer, REDIRECT_IN, 1));
	if (lexer->line[lexer->line_index] == '>')
		return (tokenize_stuffs(lexer, REDIRECT_OUT, 1));
	if (lexer->line[lexer->line_index] == '\0')
		return ((t_token){0});
	return (tokenize_stuffs(lexer, WORD, 0));
}
