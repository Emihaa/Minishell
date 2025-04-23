/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 20:56:09 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/23 20:02:18 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static
int	handle_leftover(t_arena *arena, t_string *str, t_arg *arg, t_arg *leftover)
{
	uint32_t	len;

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

static
char	*create_argument(t_arena *arena, t_arg *arg, t_arg *leftover)
{
	t_string	str;

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
			if (handle_variable(arena, &str, arg, leftover))
				break ;
		}
	}
	if (arg->exist == false)
		return (NULL);
	terminate_and_commit_string(arena, &str);
	return (str.base);
}

char	**create_argv(t_arena *arena, t_node *node)
{
	t_arg		arg_vars;
	t_arg		leftover;
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
