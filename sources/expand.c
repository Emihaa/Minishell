/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:51:33 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/19 19:35:02 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// jos ekasta expansionista tulee tyhjaa ni se argv skipataan

// LUKA\0
// $OMPPU"ASD"="LUKA\0ASD"
// expand to env, new recursion node, with env & rest of node->token.u_data.string + len
// (need to cut off the "OMPPU" from the string as well)
// expansion expands to "LUKA ASD" and field splitting recognices two words so second recursion
// still carrying the old info
// on "ASD" there is no space so no \0, and we will continue to write rest of the node->token.u_data.string +len($OMPPU)
// into the same argument
// after expansion is done, we will still move forward to node.left.left if not NULL

// maybe dest is not needed

void	copy_in_single_quote(t_arena *arena, t_arg *arg, t_string *str)
{
	uint32_t len;

	len = 0;
	arg->i += 1;
	while (arg->i + len < arg->data_len)
	{
		if (arg->data_str[arg->i + len] == '\'')
			break ;
		len += 1;
	}
	append_to_string(arena, str, &arg->data_str[arg->i], len);
	arg->i += len + 1;
}

void	copy_in_double_quote(t_arena *arena, t_arg *arg, t_string *str)
{
	uint32_t len;

	arg->i += 1;
	len = 0;
	while (arg->i + len < arg->data_len)
	{
		if (arg->data_str[arg->i + len] == '"')
			break ;
		if (arg->data_str[arg->i + len] == '$')
		{
			append_to_string(arena, str, &arg->data_str[arg->i], len);
			arg->i += len;
			expand_variable_in_quotes(arena, arg, str);
			len = 0;
		}
		else
			len += 1;
	}
	append_to_string(arena, str, &arg->data_str[arg->i], len);
	arg->i += len;
	if (arg->data_str[arg->i] == '"')
		arg->i += 1;
}

int	handle_leftover(t_arena *arena, t_string *str, t_arg *arg, t_arg *leftover)
{
	uint32_t len;

	len = 0;
	arg->exist = false;
	if (len < leftover->data_len)
	{
		while (!is_space(leftover->data_str[len]) && len < leftover->data_len)
			len += 1;
		append_to_string(arena, str, leftover->data_str, len);
		if (is_space(leftover->data_str[len]))
		{
			terminate_and_commit_string(arena, str);
			len += eat_space(&leftover->data_str[len]);
			leftover->data_str = &leftover->data_str[len];
			leftover->data_len = ft_strlen(&leftover->data_str[len]);
			leftover->i = 0;
			return (1);
		}
		arg->exist = true;
	}
	leftover->data_len -= len;
	return (0);
}

int copy_env_var_and_split(t_arena *arena, t_string *str, t_arg *arg, t_arg *leftover)
{
	uint32_t	key_len;
	char		*var;
	uint32_t	len;

	key_len = get_env_key_len(&arg->data_str[arg->i], arg->data_len - arg->i);
	var = get_env_var_value(&arg->data_str[arg->i], key_len);
	arg->i += key_len;
	if (var == NULL)
		return (0);
	len = 0;
	while (!is_space(var[len]) && var[len] != '\0')
		len += 1;
	append_to_string(arena, str, var, len);
	if (is_space(var[len]))
	{
		if (len > 0)
			arg->exist = true;
		len += eat_space(&var[len]);
		leftover->data_str = &var[len];
		leftover->data_len = ft_strlen(&var[len]);
		leftover->i = 0;
		return (1);
	}
	arg->exist = true;
	return (0);
}

void	handle_quote(t_arena *arena, t_arg *arg, t_string *str)
{
	if (arg->data_str[arg->i] == '\'')
		copy_in_single_quote(arena, arg, str);
	else
		copy_in_double_quote(arena, arg, str);
	arg->exist = true;
}

int handle_var(t_arena *arena, t_string *str, t_arg *arg, t_arg *leftover)
{
	if (is_valid_var_start(arg->data_str[arg->i + 1]))
	{
		arg->i += 1;
		if (copy_env_var_and_split(arena, str, arg, leftover))
			return (1);
	}
	else if (arg->data_str[arg->i + 1] == '?')
	{
		copy_exit_code(arena, arg, str);
	}
	else
	{
		append_to_string(arena, str, &arg->data_str[arg->i], 1);
		arg->i += 1;
		arg->exist = true;
	}
	return (0);
}


// luka sais fix this
char *create_argument(t_arena *arena, t_arg *arg, t_arg *leftover)
{
	t_string str;

	str = (t_string){0};
	if (handle_leftover(arena, &str, arg, leftover))
		return (str.base);
	while (arg->i < arg->data_len)
	{
		copy_until_quote_or_var(arena, arg, &str);
		if (is_quote(arg->data_str[arg->i]))
			handle_quote(arena, arg, &str);
		else if (arg->data_str[arg->i] == '$')
		{
			if (handle_var(arena, &str, arg, leftover))
				break ;
		}
	}
	if (arg->exist == false)
		return (NULL);
	terminate_and_commit_string(arena, &str); // add thing that moves entire argument to new arena chunk if not enough space
	return (str.base);
}

char	**create_argv(t_arena *arena, t_node *node)
{
	t_arg arg_vars;
	t_arg leftover;
	t_arg_vec	argv;
	char		*arg;

	arg_vars = (t_arg){0};
	leftover = (t_arg){0};
	init_argv(arena, &argv, ARGV_DEFAULT_SIZE);
	while (node)
	{
		if (leftover.data_len <= 0)
			init_arg(&node->token, &arg_vars);
		arg = create_argument(arena, &arg_vars, &leftover);
		if (arg != NULL)
		{
			if (argv.capacity <= argv.size)
				realloc_argv(arena, &argv);
			argv.data[argv.size] = arg;
			argv.size += 1;
		}
		if (leftover.data_len > 0)
			continue ;
		node = node->left;
	}
	return (argv.data);
}




// first time i go through the tree, i do expansion of the words
// field splitting and handle the quates
// go through the tree second time and gather all the words of same branch under same string
// separated by \0
// then create an array of pointers that point to the starting points of the string
int expand(t_arena *arena, t_minishell *m, t_node *tree)
{
	// char *str;
	t_node *tree_root;

	// printf("\n---- starting tree expansion ----\n");

	//loop here so that we send the first word node of branch
	//come ut of branch, return argv to first word node and then go to another branch loop
	// int i = 0;
	while (tree)
	{
		tree_root = tree;
		//find the first WORD node
		while (tree)
		{
			if (tree->token.type > 0)
			{
				tree->token.type = heredoc(arena, m, &tree->token);
				if (tree->token.type == -2)
					return (-2);
			}
			if (tree->token.type == REDIRECT_OUT || tree->token.type == REDIRECT_IN || tree->token.type == REDIRECT_APPEND)
			 	expand_redirect(arena, tree);
			if (tree->token.type == WORD)
			{
				// str = arena_alloc_no_zero(arena, sizeof(char) * 0); //alloc only the first pointer // LUKA: 3.4 took out the 1 byte slack
				tree->token.argv = create_argv(arena, tree);					   // if we encounter weird issues
				tree->left = NULL;																   // it's probably because of this
				break ;
			}
			tree = tree->left;
		}
		tree = tree_root->right;
	}
	// printf("arena size after expansion: %lu\n", arena->size);
	// printf("\n---- tree expanded ----\n\n\n");
	return (0);
}
