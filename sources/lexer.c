/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 22:33:07 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/02 00:30:15 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" // change to be a just "minishell.h" after making sure makefile linking works properly

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

bool is_token_invalid_next_token(char c)
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

t_token tokenize_pipe(void)
{
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
void syntax_error(t_lexer *lexer)
{
	char token[] = ;
	printf("minishell: syntax error near unexpected token `%s'", );
}

t_token tokenize_redir_in(t_lexer *lexer)
{
	char c;
	int len;
	
	len = 0;
	while (ft_is_white_space(lexer->line[++lexer->line_index]))
		;
	while (1)
	{
		c = lexer->line[lexer->line_index + len];
		if (is_token_invalid_next_token(c))
			syntax_error(lexer);
		if (ft_is_white_space(c))
			break;
		len += 1;
	}
	return ((t_token)
	{
		.type = REDIRECT_IN,
		.string = (t_token_string)
		{
			.length = len,
			.pointer = &lexer->line[lexer->line_index],
		}
	});
}

t_token create_next_token(t_lexer *lexer)
{
	if (ft_strncmp(&lexer->line[lexer->line_index], ">>", 2))
		return (handle_append(lexer));
	if (ft_strncmp(&lexer->line[lexer->line_index], "<<", 2))
		return (handle_heredoc(lexer));
	if (lexer->line[lexer->line_index] == '|')
		return (handle_pipe(lexer);
	if (lexer->line[lexer->line_index] == '<')
		return (handle_redir_in(lexer);
	if (lexer->line[lexer->line_index] == '>')
		return (handle_redir_out(lexer));
	if (lexer->line[lexer->line_index] == '$')
		return (handle_expansion(lexer));
	if (lexer->line[lexer->line_index] == '\'')
		return (handle_single_quote(lexer));
	if (lexer->line[lexer->line_index] == '"')
		return (handle_double_quote(lexer));
	if (lexer->line[lexer->line_index] == '\0')
		return (handle_end_of_line(lexer));
	return ; // error
}


t_token get_next_token(t_lexer *lexer)
{
	t_token token;

	while (ft_is_white_space(lexer->line[lexer->line_index]))
		lexer->line_index += 1;
	token = create_next_token(lexer);
}