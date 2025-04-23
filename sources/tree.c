/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:00:37 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/21 23:59:15 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 

/* Examples of AST tree:
cat > a > b > c                 |   cat > a > b > c | > outfile
								|
								|                  pipe
		  > a                   |                /      \
		  /                     |             > a     > outfile
	   > b                      |             /
	   /                        |         > b
	> c                         |         /
	/                           |      > c
  cat                           |      /
								|    cat

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

// Examples above:
// always adds new nodes to left side on the branch
// commands/words should be at the bottom
// redirect should be in the middle
// pipex always at the top
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

// Function to print the tree
static void print_tree(t_node *node, int depth)
{
	int i = 0; 
	if (!node)
		return ;
	while (i++ < depth)
		printf("  ");
	// Print current node
	printf("[-- ");
	print_token_type(&node->token.type);
	printf(" -> %.*s\n", (int)node->token.string_len, node->token.string);
	// Print left subtree
	if (node->left)
	{
		printf(" L ");
		print_tree(node->left, depth + 1);
	}
	// Print right subtree
	if (node->right)
	{
		printf(" R ");
		print_tree(node->right, depth + 1);
	}
}

// add before return line:
// print_tree(tree, 1);
// if you want the tree to be printed
// <-- @TODO pipe as a last argument should be syntax error maybe fixed?
// @TODO function has too many lines now
t_node	*parser(t_arena *arena, t_minishell *m, char *line)
{
	t_node *tree;
	t_lexer lexer;
	t_token token;
	t_type prev_type;

	tree = NULL;
	prev_type = ERROR;
	lexer = (t_lexer){.line = line, .line_index = 0};
	while (1)
	{
		token = get_next_token(&lexer);
		if (token.type == END_OF_LINE && prev_type == PIPE)
			return (syntax_error(m, &token, &lexer));
		if (token.type == END_OF_LINE)
			break ;
		if (heredoc_limit(&token, &m->heredoc_count))
			return (NULL);
		if ((token.string_len == 0) || (token.type == PIPE && \
			prev_type == ERROR) || (token.type == PIPE && prev_type == PIPE))
			return (syntax_error(m, &token, &lexer));
		tree = insert_node(tree, NULL, &token, arena);
		prev_type = token.type;
	}
	tree = find_head_root(tree);
	print_tree(tree, 1);
	m->heredoc_count = 0;
	if (expand(arena, m, tree)) // shiiit we need to return to the main readloop if heredoc is stopped by sigint. seems like we also need to make a sub process to get the exit value of 128 + signal
		return (NULL);
	return (tree);
}
