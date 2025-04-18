/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 21:25:00 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/18 21:20:41 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_arg(t_token *data, t_arg *arg_vars)
{
	arg_vars->data_len = data->string_len;
	arg_vars->data_str = data->string;
	arg_vars->i = 0;
	arg_vars->exist = false;
}

void	init_argv(t_arena *arena, t_arg_vec *argv, size_t cap)
{
	argv->capacity = cap;
	argv->data = xarena_alloc(arena,
			sizeof(*argv->data) * (argv->capacity + 1));
	argv->size = 0;
}

int realloc_argv(t_arena *arena, t_arg_vec *argv)
{
	argv->capacity *= 2;
	argv->data = arena_realloc(arena, argv->data,
			argv->size * sizeof(*argv->data),
			sizeof(*argv->data) * (argv->capacity + 1));
	if (argv->data == NULL)
		return (-1);
	return (0);
}

int	is_quote_or_var(char c)
{
	return(is_quote(c) || c == '$');
}
