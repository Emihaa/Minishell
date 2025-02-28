/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 22:33:07 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/01 00:23:17 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" // change to be a just "minishell.h" after making sure makefile linking works properly



t_token create_next_token(t_lexer *lexer)
{
	if (lexer->line[lexer->line_index] == '|')
	{
		return (handle_token_pipe(lexer));
	}
	if (lexer->line[lexer->line_index] == '<')
	{
		handle_token_redir_in(lexer);
		return ;
	}
	if (lexer->line[lexer->line_index] == '>')
	{
		handle_token_redir_in(lexer);
		return ;
	}
	if (lexer->line[lexer->line_index] == '\0')
		return (END_OF_LINE);
	return (-2);
}


t_token get_next_token(t_lexer *lexer)
{
	t_token token;

	while (ft_is_white_space(lexer->line[lexer->line_index]))
		lexer->line_index += 1;
	token = create_next_token(lexer);
}