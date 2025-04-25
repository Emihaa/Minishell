/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 22:33:07 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/25 18:03:39 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser.h" 

/// @brief does the current character delimit a word
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

/// @brief moves in the line until finds a matching quote or the EOL
static inline
int	match_quote(t_lexer *lexer, char quote, int len)
{
	char	c;

	if (quote == '\'' || quote == '"')
	{
		len += 1;
		while (1)
		{
			c = lexer->line[lexer->line_index + len];
			if (c == quote || c == '\0')
				break ;
			len += 1;
		}
	}
	return (len);
}

/// @brief creates a token matching the input parameters
/// for tokens that can have a string
static
t_token	tokenize(t_lexer *lexer, t_type type, int to_skip)
{
	t_token	token;
	char	c;
	int		len;

	len = 0;
	lexer->line_index += to_skip;
	lexer->line_index += eat_space(&lexer->line[lexer->line_index]);
	while (1)
	{
		c = lexer->line[lexer->line_index + len];
		if (is_quote(c))
		{
			len = match_quote(lexer, c, len);
			c = lexer->line[lexer->line_index + len];
		}
		if (is_delimiter(c))
			break ;
		len += 1;
	}
	token.type = type;
	token.string_len = len;
	token.string = &lexer->line[lexer->line_index];
	lexer->line_index += len;
	return (token);
}

/// @brief returns a token with correct parameters for a pipe
static inline
t_token	tokenize_pipe(t_lexer *lexer)
{
	const t_token	token = {
		.type = PIPE,
		.string = &lexer->line[lexer->line_index],
		.string_len = 1,
	};

	lexer->line_index += 1;
	return (token);
}

/// @TODO:	find out which character can be valid parts of a word
// 			currently anything other than a delimiter character counts as a word
//			which doesn't seem correct
/// @brief skips whitespace between tokens and 
/// finds the next token type to return
t_token	get_next_token(t_lexer *lexer)
{
	while (is_space(lexer->line[lexer->line_index]))
		lexer->line_index += 1;
	if (ft_strncmp(&lexer->line[lexer->line_index], ">>", 2) == 0)
		return (tokenize(lexer, REDIRECT_APPEND, 2));
	if (ft_strncmp(&lexer->line[lexer->line_index], "<<", 2) == 0)
		return (tokenize(lexer, HERE_DOCUMENT, 2));
	if (lexer->line[lexer->line_index] == '|')
		return (tokenize_pipe(lexer));
	if (lexer->line[lexer->line_index] == '<')
		return (tokenize(lexer, REDIRECT_IN, 1));
	if (lexer->line[lexer->line_index] == '>')
		return (tokenize(lexer, REDIRECT_OUT, 1));
	if (lexer->line[lexer->line_index] == '\0')
		return ((t_token){0});
	return (tokenize(lexer, WORD, 0));
}
