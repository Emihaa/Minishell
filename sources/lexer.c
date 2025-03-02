/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 22:33:07 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/02 21:48:03 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" // change to be a just "minishell.h" after making sure makefile linking works properly

// propably bad reuse of code
// @TODO: re make this
void syntax_error(t_lexer *lexer)
{
	char *token;

	if (!ft_strncmp(&lexer->line[lexer->line_index], ">>", 2))
		token = ((char [3]){'>', '>', '\0'}); 
	else if (!ft_strncmp(&lexer->line[lexer->line_index], "<<", 2))
		token = ((char [3]){'<', '<', '\0'});
	else if (lexer->line[lexer->line_index] == '|')
		token = ((char [2]){'|', '\0'});
	else if (lexer->line[lexer->line_index] == '<')
		token = ((char [2]){'<', '\0'});
	else if (lexer->line[lexer->line_index] == '>')
		token = ((char [2]){'>', '\0'});
	else if (lexer->line[lexer->line_index] == '$')
		token = ((char [2]){'$', '\0'});
	else if (lexer->line[lexer->line_index] == '\'')
		token = ((char [2]){'\'', '\0'});
	else if (lexer->line[lexer->line_index] == '"')
		token = ((char [2]){'"', '\0'});
	else if (lexer->line[lexer->line_index] == '\0')
		token = ((char [8]){'n', 'e', 'w', 'l', 'i', 'n', 'e', '\0'});
	else 
		return ; // error
	printf("minishell: syntax error near unexpected token `%s'\n", token);
}

// bool is_token_char(t_lexer *lexer)
// {
// 	if (ft_strncmp(&lexer->line[lexer->line_index], ">>", 2))
// 		return (true);
// 	if (ft_strncmp(&lexer->line[lexer->line_index], "<<", 2))
// 		return (true);
// 	if (lexer->line[lexer->line_index] == '|')
// 		return (true);
// 	if (lexer->line[lexer->line_index] == '<')
// 		return (true);
// 	if (lexer->line[lexer->line_index] == '>')
// 		return (true);
// 	if (lexer->line[lexer->line_index] == '$')
// 		return (true);
// 	if (lexer->line[lexer->line_index] == '\'')
// 		return (true);
// 	if (lexer->line[lexer->line_index] == '"')
// 		return (true);
// 	if (lexer->line[lexer->line_index] == '\0')
// 		return (true);
// }

bool is_invalid_next_token(char c)
{
	if (c == '|')
		return (true);
	if (c == '<')
		return (true);
	if (c == '>')
		return (true);
	if (c == '$')
		return (true);
	if (c== '\'')
		return (true);
	if (c == '"')
		return (true);
	if (c == '\0')
		return (true);
	return (false);
}

// non one line version
// let's use this one if the other one seems not readable

// t_token tokenize_pipe(t_lexer *lexer)
// {
// 	t_token token;

// 	token.type = PIPE;
// 	token.string = (t_token_string){
// 		.length = 1,
// 		.pointer = &lexer->line[lexer->line_index]
// 	};
// 	return (token);
// }

t_token tokenize_pipe(t_lexer *lexer)
{
	lexer->line_index += 1;
	return ((t_token){
		.type = PIPE,
		.string = {
			.length = 0,
			.pointer = NULL,
			}
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
		}
	});
}

t_token tokenize_redir_in(t_lexer *lexer)
{
	t_token token;
	char c;
	int len;
	
	len = 0;
	while (ft_is_white_space(lexer->line[lexer->line_index]))
		lexer->line_index += 1;
	while (1)
	{
		c = lexer->line[lexer->line_index + len];
		if (is_invalid_next_token(c))
		{
			syntax_error(lexer);
			break ;
		}
		if (ft_is_white_space(c) || c == '\0')
			break;
		len += 1;
	}
	token = (t_token){
		.type = REDIRECT_IN,
		.string = (t_token_string){
			.length = len,
			.pointer = &lexer->line[lexer->line_index],
		}
	};
	lexer->line_index += len;
	return (token);
}

t_token tokenize_redir_out(t_lexer *lexer)
{
	t_token token;
	char c;
	int len;
	
	len = 0;
	while (ft_is_white_space(lexer->line[lexer->line_index]))
		lexer->line_index += 1;
	while (1)
	{
		c = lexer->line[lexer->line_index + len];
		if (is_invalid_next_token(c))
		{
			syntax_error(lexer);
			break ;
		}
		if (ft_is_white_space(c) || c == '\0')
			break;
		len += 1;
	}
	token = (t_token){
		.type = REDIRECT_OUT,
		.string = (t_token_string){
			.length = len,
			.pointer = &lexer->line[lexer->line_index],
		}
	};
	lexer->line_index += len;
	return (token);
}

t_token create_next_token(t_lexer *lexer)
{
	// if (ft_strncmp(&lexer->line[lexer->line_index], ">>", 2))
	// 	return (handle_append(lexer));
	// if (ft_strncmp(&lexer->line[lexer->line_index], "<<", 2))
	// 	return (handle_heredoc(lexer));
	if (lexer->line[lexer->line_index] == '|')
		return (tokenize_pipe(lexer));
	if (lexer->line[lexer->line_index] == '<')
		return (tokenize_redir_in(lexer));
	if (lexer->line[lexer->line_index] == '>')
		return (tokenize_redir_out(lexer));
	// if (lexer->line[lexer->line_index] == '$')
	// 	return (handle_expansion(lexer));
	// if (lexer->line[lexer->line_index] == '\'')
	// 	return (handle_single_quote(lexer));
	// if (lexer->line[lexer->line_index] == '"')
	// 	return (handle_double_quote(lexer));
	if (lexer->line[lexer->line_index] == '\0')
		return (tokenize_end_of_line());
	return ((t_token){.type = ERROR, .string = {0}}); // error
}

t_token get_next_token(t_lexer *lexer)
{
	t_token token;

	while (ft_is_white_space(lexer->line[lexer->line_index]))
		lexer->line_index += 1;
	token = create_next_token(lexer);
	return (token);
}


//testing main
#include <string.h>
void print_token(t_lexer *lexer)
{
	while (1)
	{

		t_token token = get_next_token(lexer);
		char *token_string = calloc(1, token.string.length + 1	);
		memmove(token_string, token.string.pointer, token.string.length);
		printf("token number: %i\ttoken string: %s\n", token.type, token_string);
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