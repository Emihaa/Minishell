/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:00:37 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/25 17:51:30 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/expand.h"
#include "../includes/parser.h"

/* Examples of AST tree:
			cat > a > b > c 
										
					> a 
					/ 
				> b 
				/ 
			> c  
			/ 
		cat 
								
	cat > a > b > c | > outfile

                 pipe
                /      \
             > a     > outfile
             /
         > b
         /			 
      > c
      /
    cat

ls > a > b > c | cat | > outfile wc -l | echo smth > 2

					pipe1
					/   \
				> a     pipe2
				/       /   \
			> b     cat     pipe3
			/               /   \
		> c         > outfile   > 2
		/               /       /
	   ls            wc -l   echo smth
								
*/

/// @brief Examples above:
/// always adds new nodes to left side on the branch
/// commands/words should be at the bottom
/// redirect should be in the middle
/// pipex always at the top
t_node	*insert_node(t_node *node, t_node *root, t_token *token, t_arena *arena)
{
	if (!node)
		return (create_node(root, token, arena));
	else if (token->type == WORD)
	{
		if (node->token.type == PIPE)
			return (node->right = create_node(node, token, arena));
		return (insert_bottom(node, token, arena));
	}
	else if (token->type == PIPE)
		return (insert_top(node, token, arena));
	else
	{
		if (node->token.type == PIPE)
			return (node->right = create_node(node, token, arena));
		else if (node->token.type == WORD)
			return (insert_middle(node->root, node, token, arena));
		else
			return (insert_below(node, node, token, arena));
	}
	return (node);
}

/// @brief syntax handling and AST (Abstract syntax tree) start
/// returns either the pointer to the root of the tree
/// or NULL
t_node	*parser(t_arena *arena, t_minishell *m, char *line)
{
	t_parser	p;

	p.tree = NULL;
	p.prev = ERROR;
	p.lexer = (t_lexer){.line = line, .line_index = 0};
	while (1)
	{
		p.token = get_next_token(&p.lexer);
		if (p.token.type == END_OF_LINE && p.prev == PIPE)
			return (syntax_error(m, &p.token, &p.lexer));
		if (p.token.type == END_OF_LINE)
			break ;
		if (heredoc_limit(&p.token, &m->heredoc_count))
			return (NULL);
		if ((p.token.string_len == 0) || (p.token.type == PIPE
				&& p.prev == ERROR) || (p.token.type == PIPE && p.prev == PIPE))
			return (syntax_error(m, &p.token, &p.lexer));
		p.tree = insert_node(p.tree, NULL, &p.token, arena);
		p.prev = p.token.type;
	}
	p.tree = find_head_root(p.tree);
	m->heredoc_count = 0;
	if (expand(arena, m, p.tree))
		return (NULL);
	return (p.tree);
}
