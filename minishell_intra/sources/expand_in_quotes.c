/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_in_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 20:45:59 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/25 00:51:17 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/expand.h"
#include "../includes/builtin.h"

/// @brief copies from source arg the text within singlequotes into str
/// @param arena used for allocations when necessary
/// @param arg source data to be copied from
/// @param str destinaton to copy to
static void	copy_in_single_quote(t_arena *arena, t_arg *arg, t_string *str)
{
	uint32_t	len;

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

/// @brief	copies from source arg the text within singlequotes into str
///			and moves argument index forward by the length of the key
/// @param arena used for allocations when necessary
/// @param arg source data to be copied from
/// @param str destinaton to copy to
static void	copy_full_env_var(t_arena *arena, t_arg *arg, t_string *str)
{
	uint32_t	key_len;
	uint32_t	var_len;
	char		*env_var;

	key_len = get_env_key_len(&arg->data_str[arg->i], arg->data_len - arg->i);
	env_var = get_env_var_value(&arg->data_str[arg->i], key_len);
	if (env_var != NULL)
	{
		var_len = ft_strlen(env_var);
		append_to_string(arena, str, env_var, var_len);
	}
	arg->i += key_len;
}

/// @brief helper to determine which function to call after encountering a $
/// @param arena used for allocations when necessary
/// @param str where to copy data to
/// @param arg source data to be copied from
static void	handle_variable_in_quotes(t_arena *arena, t_arg *arg, t_string *str)
{
	if (is_valid_var_start(arg->data_str[arg->i + 1]))
	{
		arg->i += 1;
		copy_full_env_var(arena, arg, str);
	}
	else if (arg->data_str[arg->i + 1] == '?')
	{
		copy_exit_code(arena, arg, str);
	}
	else
	{
		append_to_string(arena, str, &arg->data_str[arg->i], 1);
		arg->i += 1;
	}
}

// copies from source arg to str within double quotes
// if a $ is found copies the whole env variable
// and moving index forward by the length of the key
// need arena to reallocate str if necessary

/// @brief	copies until a $ then expands the variable
///			and continues until a delimiting quote or end of string
/// @param arena used for allocations when necessary
/// @param str where to copy data to
/// @param arg source data to be copied from
static void	copy_in_double_quote(t_arena *arena, t_arg *arg, t_string *str)
{
	uint32_t	len;

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
			handle_variable_in_quotes(arena, arg, str);
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

/// @brief call the correct function depending on the current char
/// @param arena used for allocations when necessary
/// @param str where to copy data to
/// @param arg source data to be copied from
void	handle_quote(t_arena *arena, t_arg *arg, t_string *str)
{
	if (arg->data_str[arg->i] == '\'')
		copy_in_single_quote(arena, arg, str);
	else
		copy_in_double_quote(arena, arg, str);
	arg->exist = true;
}
