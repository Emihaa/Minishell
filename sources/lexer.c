/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 22:33:07 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/03 18:37:24 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// to compile just the lexer for testing use
// 
/*
cc lexer.c -l readline ../libs/libft/build/libft.a ../libs/lt_alloc/build/lt_alloc.a -o lexer

*/
#include "../includes/minishell.h" 
// change to be a just "minishell.h" after making sure makefile linking works properly

// LUKA: should the lexer care about syntax errors or not
// seems like probably not
// probably makes more sense to detect syntax error in parser when getting tokens in unexpected order

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

// @question which style is nicer :P?

// just returns a token with correct parameters for a pipe
static inline
t_token	tokenize_pipe(t_lexer *lexer)
{
	const t_token token = {
		.type = PIPE,
		.u_data.string = &lexer->line[lexer->line_index],
		.string_len = 1,
	};
	lexer->line_index += 1;
	return (token);
}

// just returns a token with correct parameters for a EOL
static inline
t_token	tokenize_end_of_line(void)
{
	return ((t_token)
		{
			.type = END_OF_LINE,
			.u_data.string = NULL,
			.string_len = 0,
		});
}

// creates a token matching the input parameters
// for tokens that can have a string
// @TODO: rename
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
		len = match_quote(lexer, c, len); // if we hit a quote len will for sure be at least 1 so might break the len check 
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
		return (tokenize_end_of_line());
	return (tokenize_stuffs(lexer, WORD, 0));
}

// use to get an array of all the tokens in a line
// give an arena to allocate the array with
// give an initialized lexer
// t_token *get_token_array(t_arena *arena, t_lexer *lexer)
// {
// 	t_token *token_array_base;
// 	int	i;

// 	token_array_base = arena_alloc(arena, sizeof(t_token));
// 	i = 0;
// 	while (1)
// 	{
// 		token_array_base[i] = get_next_token(lexer);
// 		if (token_array_base[i].type == END_OF_LINE)
// 			break ;
// 		arena_alloc(arena, sizeof(t_token));
// 		i++;
// 	}
// 	return (token_array_base);
// }
