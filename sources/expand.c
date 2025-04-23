/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:51:33 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/23 22:37:32 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/// @brief helper to assign the leftover
/// @param var environment variable pointer
/// @param var_index current index inside the variable
/// @param leftover leftover is updated with new values
static inline
void	assign_leftover(char *var, uint32_t var_index, t_arg *leftover)
{
	var_index += eat_space(&var[var_index]);
	leftover->data_str = &var[var_index];
	leftover->data_len = ft_strlen(&var[var_index]);
	leftover->i = 0;
}

/// @brief	appends enviroment variable to str until a space is encountered
///			at which point the leftover is set and status is returned
/// @param arena used for allocations when necessary
/// @param str where to copy data to
/// @param arg source data to be copied from
/// @param leftover leftover from fieldsplitting is set here
/// @return did we encounter an enviroment variable that caused field splitting 
int	copy_env_var_and_split(t_arena *arena, t_string *str,
	t_arg *arg, t_arg *leftover)
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
		if (leftover != NULL)
			assign_leftover(var, len, leftover);
		return (1);
	}
	arg->exist = true;
	return (0);
}

/// @brief helper to determine which function to call and flags to set
/// @param arena used for allocations when necessary
/// @param str where to copy data to
/// @param arg source data to be copied from
/// @param leftover leftover from fieldsplitting is set here
/// @return did we encounter an enviroment variable that caused field splitting 
int	handle_variable(t_arena *arena, t_string *str, t_arg *arg, t_arg *leftover)
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
		arg->exist = true;
	}
	else
	{
		append_to_string(arena, str, &arg->data_str[arg->i], 1);
		arg->i += 1;
		arg->exist = true;
	}
	return (0);
}

/// @brief append from source arg to str until a char needing 
///			special handling is encountered
/// @param arena used for allocations when necessary
/// @param str where to copy data to
/// @param arg source data to be copied from
void	copy_until_quote_or_var(t_arena *arena, t_arg *arg, t_string *str)
{
	uint32_t	len;

	len = 0;
	while (arg->i + len < arg->data_len)
	{
		if (is_quote_or_var(arg->data_str[arg->i + len]))
			break ;
		len += 1;
	}
	if (len > 0)
		arg->exist = true;
	append_to_string(arena, str, &arg->data_str[arg->i], len);
	arg->i += len;
}

// loop here so that we send the first word node of branch
// come ut of branch, return argv to first word node
// and then go to another branch loop
int	expand(t_arena *arena, t_minishell *m, t_node *tree)
{
	t_node	*tree_root;

	while (tree)
	{
		tree_root = tree;
		while (tree)
		{
			if (tree->token.type > 0)
				if (heredoc(arena, m, &tree->token) == -2)
					return (-2);
			if (tree->token.type == REDIRECT_OUT
				|| tree->token.type == REDIRECT_IN
				|| tree->token.type == REDIRECT_APPEND)
				expand_redirect(arena, tree);
			if (tree->token.type == WORD)
			{
				tree->token.argv = create_argv(arena, tree);
				tree->left = NULL;
				break ;
			}
			tree = tree->left;
		}
		tree = tree_root->right;
	}
	// printf("arena size after expansion: %lu\n", arena->size);
	return (0);
}
