/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 21:25:00 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/30 17:00:45 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/expand.h"

/// @brief copy the current arguments data to the struct
void	init_arg(t_token *data, t_arg *arg_vars)
{
	arg_vars->data_len = data->string_len;
	arg_vars->data_str = data->string;
	arg_vars->i = 0;
	arg_vars->exist = false;
}

/// @brief initialize argv.
/// @param capacity = default_size
/// @param data = arena allocated.
/// @param size = 0;
void	init_argv(t_arena *arena, t_arg_vec *argv, size_t cap)
{
	argv->capacity = cap;
	argv->data = xarena_alloc(arena,
			sizeof(*argv->data) * (argv->capacity + 1));
	argv->size = 0;
}

void	realloc_argv(t_arena *arena, t_arg_vec *argv)
{
	argv->capacity *= 2;
	argv->data = xarena_realloc(arena, argv->data,
			argv->size * sizeof(*argv->data),
			sizeof(*argv->data) * (argv->capacity + 1));
}

/// @brief if char is quote or '$'
int	is_quote_or_var(char c)
{
	return (is_quote(c) || c == '$');
}
