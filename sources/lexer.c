/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 22:33:07 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/03 20:17:09 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" // change to be a just "minishell.h" after making sure makefile linking works properly

// LUKA: should the lexer care about syntax errors or not
// seems like propably not
// propably makes more sense to detect syntax error in parser when getting tokens in unexpected order

bool is_delimiter(char c)
{
	if (c == '|')
		return (true);
	if (c == '<')
		return (true);
	if (c == '>')
		return (true);
	if (c == '\0')
		return (true);
	return (false);
}

int match_quote(t_lexer *lexer, char quote, int len)
{
	char c;
	int	i;

	if (quote == '\'' || quote == '"')
	{
		i = len + 1;
		while (1)
		{
			c = (lexer->line[lexer->line_index + i]);
			if (c == quote || c == '\0')
				break;
			i += 1;
		}
		return (i);
	}
	return (len);
}

t_token tokenize_pipe(t_lexer *lexer)
{
	lexer->line_index += 1;
	return ((t_token){
		.type = PIPE,
		.string = {
			.length = 0,
			.pointer = NULL,
			},
		.name = PIPE_NAME,
		});
}

t_token tokenize_end_of_line(void)
{
	return ((t_token)
	{
		.type = END_OF_LINE,
		.string = (t_token_string) 
		{
			.length = 0,
			.pointer = NULL,
		},
		.name = EOL_NAME,
	});
}


t_token tokenize_stuffs(t_lexer *lexer, t_token_type token_type, char *token_name, int to_skip) // @TODO: rename
{
	t_token token;
	char c;
	int len;
	
	len = 0;
	lexer->line_index += to_skip;
	while (ft_is_white_space(lexer->line[lexer->line_index])) // propably change to just ' ' && '\t' maybe '\n'
		lexer->line_index += 1;
	while (1)
	{
		c = lexer->line[lexer->line_index + len];
		len = match_quote(lexer, c, len);
		if (is_delimiter(c))
			break ;
		if (ft_is_white_space(c) || c == '\0')
			break;
		len += 1;
	}
	token = (t_token){
		.type = token_type,
		.string = (t_token_string){
			.length = len,
			.pointer = &lexer->line[lexer->line_index],
		},
		.name = token_name,
	};
	lexer->line_index += len;
	return (token);
}

t_token get_next_token(t_lexer *lexer)
{	
	while (ft_is_white_space(lexer->line[lexer->line_index]))
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


//testing main
#include <string.h>
void print_token(t_lexer *lexer)
{
	while (1)
	{

		t_token token = get_next_token(lexer);
		char *token_string = calloc(1, token.string.length + 1);
		memmove(token_string, token.string.pointer, token.string.length);
		printf("token number: %i\ttoken name: %s\ttoken string: %s\n", token.type, token.name, token_string);
		free(token_string);
		if (token.type == END_OF_LINE || token.type == ERROR)
			break ;
	}
}


void read_loop(char **envp)
{
	char *line;
	(void)envp;
	while (1)
	{
		t_lexer lexer = {0};
		line = readline("minishell> ");
		add_history(line);
    	//printf("%s", line);
		lexer.line = line;
		print_token(&lexer);
	}
}

int main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	read_loop(envp);
	printf("exit\n");
    return (0);
}