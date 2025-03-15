/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 22:33:07 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/15 18:23:20 by ltaalas          ###   ########.fr       */
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

static inline
bool	is_space(char c)
{
	return (c == ' ' || c == '\t'); // || c == '\n' maybe?
}

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
	lexer->line_index += 1;
	return ((t_token){
		.type = PIPE,
		.string = &lexer->line[lexer->line_index - 1],
		.string_len = 1,
		.name = PIPE_NAME,
	});
}

// just returns a token with correct parameters for a EOL
static inline
t_token	tokenize_end_of_line(void)
{
	return ((t_token)
		{
			.type = END_OF_LINE,
			.string = NULL,
			.string_len = 0,
			.name = EOL_NAME,
		});
}

// creates a token matching the input parameters
// for tokens that can have a string
// @TODO: rename
t_token	tokenize_stuffs(t_lexer *lexer, t_type type, char *name, int to_skip)
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
		token.string = &lexer->line[lexer->line_index],
		token.string_len = len,
		token.name = name,
	};
	lexer->line_index += len;
	return (token);
}

// @example
// stuff necessary for calling the lexer
/*
		t_lexer lexer;
		lexer.line = readline("minishell >");
		lexer.line_index = 0;

		t_token token;
		token = get_next_token(&lexer);

*/

// skips whitespace between tokens and finds the next token type to return
// @TODO:	find out which character can be valid parts of a word
// 			currently anything other than a delimiter character counts as a word
//			which doesn't seem correct
t_token	get_next_token(t_lexer *lexer)
{	
	while (is_space(lexer->line[lexer->line_index]))
		lexer->line_index += 1;
	if (ft_strncmp(&lexer->line[lexer->line_index], ">>", 2) == 0)
		return (tokenize_stuffs(lexer, REDIRECT_APPEND, APPEND_NAME, 2));
	if (ft_strncmp(&lexer->line[lexer->line_index], "<<", 2) == 0)
		return (tokenize_stuffs(lexer, HERE_DOCUMENT, HEREDOC_NAME, 2));
	if (lexer->line[lexer->line_index] == '|')
		return (tokenize_pipe(lexer));
	if (lexer->line[lexer->line_index] == '<')
		return (tokenize_stuffs(lexer, REDIRECT_IN, REDIRECT_IN_NAME, 1));
	if (lexer->line[lexer->line_index] == '>')
		return (tokenize_stuffs(lexer, REDIRECT_OUT, REDIRECT_OUT_NAME, 1));
	if (lexer->line[lexer->line_index] == '\0')
		return (tokenize_end_of_line());
	return (tokenize_stuffs(lexer, WORD, WORD_NAME, 0));
	//return ((t_token){.type = ERROR, .string = {0}}); // error	
}

// use to get an array of all the tokens in a line
// give an arena to allocate the array with
// give an initialized lexer
t_token *get_token_array(t_arena *arena, t_lexer *lexer)
{
	t_token *token_array_base;
	int	i;

	token_array_base = arena_alloc(arena, sizeof(t_token));
	i = 0;
	while (1)
	{
		token_array_base[i] = get_next_token(lexer);
		if (token_array_base[i].type == END_OF_LINE)
			break ;
		arena_alloc(arena, sizeof(t_token));
		i++;
	}
	return (token_array_base);
}

// TESTING STUFF
// comment out when integrating to project or testing other stuff that need the lexer 
// and delete when ready to submit
// cc lexer.c -l readline ../libs/libft/build/libft.a
//testing main
void print_tokens(t_lexer *lexer)
{
	while (1)
	{
		t_token token = get_next_token(lexer);

		char *token_string = calloc(1, token.string_len + 1);
		ft_memmove(token_string, token.string, token.string_len);
		printf("token number: %i\ttoken name: %s\ttoken string: %s\n", token.type, token.name, token_string);
		free(token_string);
		if (token.type == END_OF_LINE || token.type == ERROR)
			break ;
	}
}

// void print_token_array(t_arena *arena, t_lexer *lexer)
// {
// 	int i = 0;
// 	t_token *token_array = get_token_array(arena, lexer);
// 	while (1)
// 	{
// 		t_token token = token_array[i];
// 		char *token_string = calloc(1, token.string_len + 1);
// 		ft_memmove(token_string, token.string, token.string_len);
// 		printf("token number: %i\ttoken name: %s\ttoken string: %s\n", token.type, token.name, token_string);
// 		free(token_string);
// 		if (token.type == END_OF_LINE || token.type == ERROR)
// 			break ;
// 		i++;
// 	}
// 	arena_reset(arena);
// }

// void read_loop(char **envp)
// {
// 	//t_arena arena = arena_new(DEFAULT_ARENA_CAPACITY);
// 	char *line;
// 	(void)envp;

// 	while (1)
// 	{
// 		t_lexer lexer = {0};
// 		line = readline("lexer[TEST]> ");
// 		if (line == NULL)
// 		{
// 			break ;
// 		}
// 		add_history(line);
//     	//printf("%s", line);
// 		lexer.line = line;
// 			print_tokens(&lexer);
// 		//print_token_array(&arena, &lexer);
// 		free(line);
// 	}
// }

// int main_test(int argc, char *argv[], char *envp[])
// {
// 	(void)argc;
// 	(void)argv;
// 	read_loop(envp);
// 	//char buf[1024] = {0};
// 	// while (1)
// 	// {
// 	// 	memset(buf, 0, 1024);
// 	// 	if (read(STDIN_FILENO, buf, 1023) < 1)
// 	// 		break;
// 	// 	add_history(buf);
// 	// 	write(STDOUT_FILENO, buf, ft_strlen(buf));
// 	// }

// 	printf("exit\n");
//     return (0);
// }


// int main_test(int argc, char *argv[], char *envp[])
// {
// 	(void)argc;
// 	(void)argv;
// 	// read_loop(envp);
// 	printf("exit\n");
//     return (0);
// }
