/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 01:03:22 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/25 18:16:39 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

/// @brief Struct for token data
/// @param type			<- the type of the token
/// @param string_len	<- length of the string
/// @param string
/// @param argv			<- the string of the token | 
// if applicable will be a filename a command name or an argument
typedef struct s_token
{
	t_type		type;
	uint32_t	string_len;
	union
	{
		char	*string;
		char	**argv;
	};
}	t_token;

/// @brief AST (Abstract syntax tree) data. 
/// @param token		<- t_token token struct data
/// @param root 		<- previus node this node is connected to
/// @param left			<- node to left of this node
/// @param right		<- node to right of this node
typedef struct s_node
{
	t_token			token;
	struct s_node	*root;
	struct s_node	*left;
	struct s_node	*right;
}	t_node;

/// @brief Struct for lexer
typedef struct s_lexer
{
	char		*line;
	uint32_t	line_index;
}	t_lexer;

/// @brief AST parser struct for error syntax handling
typedef struct s_parser
{
	t_node	*tree;
	t_lexer	lexer;
	t_token	token;
	t_type	prev;
}	t_parser;

// minishell_exec.c
int		minishell_exec_loop(t_arena *arena, t_minishell *m, t_node *tree);

// lexer.c
t_token	get_next_token(t_lexer *lexer);

// tree_nodes.c
t_node	*create_node(t_node *root, t_token *token, t_arena *arena);
t_node	*insert_middle(t_node *root, t_node *node,
			t_token *token, t_arena *arena);
t_node	*insert_below(t_node *root, t_node *node,
			t_token *token, t_arena *arena);
t_node	*insert_bottom(t_node *node, t_token *token, t_arena *arena);
t_node	*insert_top(t_node *node, t_token *token, t_arena *arena);

// tree_utils.c
void	print_token_type(t_type *token_type);
t_node	*find_word_root(t_node *node);
t_node	*find_head_root(t_node *node);
void	*syntax_error(t_minishell *m, t_token *token, t_lexer *lexer);
bool	heredoc_limit(t_token *token, uint32_t *heredoc_count);

// tree.c
t_node	*insert_node(t_node *node, t_node *root,
			t_token *token, t_arena *arena);
t_node	*parser(t_arena *arena, t_minishell *m, char *line);

#endif