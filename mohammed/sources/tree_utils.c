/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:00:37 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/05/06 15:24:23 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser.h"

/// @brief token recognisation function
void	print_token_type(t_type *token_type)
{
	if (*token_type == PIPE)
		put_str(2, "|");
	else if (*token_type == REDIRECT_IN)
		put_str(2, "<");
	else if (*token_type == REDIRECT_OUT)
		put_str(2, ">");
	else if (*token_type == HERE_DOCUMENT)
		put_str(2, "<<");
	else if (*token_type == REDIRECT_APPEND)
		put_str(2, ">>");
	else if (*token_type == WORD)
		put_str(2, "WORD");
	else if (*token_type == END_OF_LINE)
		put_str(2, "newline");
	else if (*token_type == ERROR)
		put_str(2, "error");
}

/// @brief find the top most word
t_node	*find_word_root(t_node *node)
{
	t_node	*temp;

	temp = node->root;
	while (temp->root && temp->root->token.type == WORD)
	{
		temp = temp->root;
	}
	return (temp);
}

/// @brief finds the main root head of tree
t_node	*find_head_root(t_node *node)
{
	t_node	*temp;

	temp = node;
	while (temp && temp->root)
	{
		temp = temp->root;
	}
	return (temp);
}

/// @brief prints syntax error & returns NULL
void	*syntax_error(t_minishell *m, t_token *token, t_lexer *lexer)
{
	put_str(2, "minishell: syntax error near unexpected token `");
	if (token->type != PIPE)
		*token = get_next_token(lexer);
	print_token_type(&token->type);
	put_str(2, "'\n");
	m->exit_status = 2;
	return (NULL);
}

/// @brief heredoc max limit for over 16 heredocs
bool	heredoc_limit(t_token *token, uint32_t *heredoc_count)
{
	if (token->type == HERE_DOCUMENT)
		(*heredoc_count)++;
	if (*heredoc_count > 16)
	{
		put_str(2, "minishell: maximum here-document count exceeded\n");
		return (true);
	}
	return (false);
}
