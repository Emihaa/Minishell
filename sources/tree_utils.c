/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:00:37 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/05 19:25:37 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 

// token recognisation function
void print_token_type(t_type *token_type)
{
	if (*token_type == PIPE)
		ft_putstr_fd("|", 2);
	else if (*token_type == REDIRECT_IN)
		ft_putstr_fd("<", 2);
	else if (*token_type == REDIRECT_OUT)
		ft_putstr_fd(">", 2);
	else if (*token_type == HERE_DOCUMENT)
		ft_putstr_fd("<<", 2);
	else if (*token_type == REDIRECT_APPEND)
		ft_putstr_fd(">>", 2);
	else if (*token_type == WORD)
		ft_putstr_fd("WORD", 2);
	else if (*token_type == END_OF_LINE)
		ft_putstr_fd("newline", 2);
	else if (*token_type == ERROR)
		ft_putstr_fd("error", 2);
}

// finds the main root head of tree
t_node *find_head_root(t_node *node)
{
	if (node && node->root)
		return (find_head_root(node->root));
	return(node);  
}

// prints syntax error
void *syntax_error(t_token *token, t_lexer *lexer)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (token->type != PIPE)
		*token = get_next_token(lexer);
	print_token_type(&token->type);
	ft_putstr_fd("'\n", 2);
	return (NULL);
}

// heredoc max limit for over 16 heredocs
bool heredoc_limit(t_token *token, uint32_t *heredoc_count)
{	
	if (token->type == HERE_DOCUMENT)
		(*heredoc_count)++; // LUKA: this used to be *heredoc_count++; this moved the pointer heredoc_count and then dereferenced it!
	if (*heredoc_count > 16)
	{
		ft_putstr_fd("minishell: maximum here-document count exceeded\n", 2);
		return (true);
	}
	return (false);
}
