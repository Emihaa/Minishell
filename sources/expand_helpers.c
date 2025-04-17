/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 21:25:00 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/17 21:35:24 by ltaalas          ###   ########.fr       */
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

void	init_argv(t_arena *arena, t_arg_vec *argv)
{
	argv->capacity = ARGV_DEFAULT_SIZE;
	argv->data = xarena_alloc(arena,
			sizeof(*argv->data) * (argv->capacity + 1));
	argv->size = 0;
}
int	is_special_char(char c)
{
	return(is_quote(c) || c == '$');
}
