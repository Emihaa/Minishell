/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_nodes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:00:37 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/23 20:32:21 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 

// create a new node
t_node	*create_node(t_node *root, t_token *token, t_arena *arena)
{
	t_node	*new_node;

	new_node = arena_alloc(arena, sizeof(t_node));
	new_node->token = *token;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->root = root;
	return (new_node);
}

// create node in between the root and current node
t_node	*insert_middle(t_node *root, t_node *node,
						t_token *token, t_arena *arena)
{
	t_node	*new_node;

	if (node->root != NULL && node->root->token.type == WORD)
	{
		root = find_word_root(node)->root;
		node = find_word_root(node);
	}
	new_node = create_node(root, token, arena);
	if (root)
	{
		if (root->token.type == PIPE && root->left)
			root->right = new_node;
		else
		{
			root->left = new_node;
			root->right = NULL;
		}
	}
	node->root = new_node;
	if (!new_node->left)
		new_node->left = node;
	else
		new_node->right = node;
	return (node);
}

// create node below current to the left
t_node	*insert_below(t_node *root, t_node *node,
						t_token *token, t_arena *arena)
{
	t_node	*new_node;

	new_node = create_node(root, token, arena);
	node->left = new_node;
	return (new_node);
}

// first find most bottom node
// then creates new node to bottom and connect the old bottom with new
t_node	*insert_bottom(t_node *node, t_token *token, t_arena *arena)
{
	t_node	*bottom_node;
	t_node	*new_node;

	bottom_node = node;
	while (bottom_node->left)
		bottom_node = bottom_node->left;
	new_node = create_node(bottom_node, token, arena);
	bottom_node->left = new_node;
	return (new_node);
}

// if token is pipe, we want the pipe to top of everything
// if at the top there is already pipe then we want the new pipe at the
// right side of the old pipe
t_node	*insert_top(t_node *node, t_token *token, t_arena *arena)
{
	t_node	*head_node;
	t_node	*new_node;
	t_node	*temp_root;

	head_node = find_head_root(node);
	if (head_node->token.type != PIPE)
	{
		new_node = create_node(NULL, token, arena);
		new_node->left = head_node;
		head_node->root = new_node;
	}
	else
	{
		temp_root = head_node;
		while (temp_root->right && temp_root->right->token.type == PIPE)
		{
			temp_root = temp_root->right;
		}
		new_node = create_node(temp_root, token, arena);
		new_node->left = temp_root->right;
		temp_root->right = new_node;
		if (new_node->left)
			new_node->left->root = new_node;
	}
	return (new_node);
}
