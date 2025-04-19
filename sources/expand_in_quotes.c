/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_in_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 20:45:59 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/19 23:17:12 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

void	copy_full_env_var(t_arena *arena, t_arg *arg, t_string *str)
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

void handle_variable_in_quotes(t_arena *arena, t_arg *arg, t_string *str)
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

void	handle_quote(t_arena *arena, t_arg *arg, t_string *str)
{
	if (arg->data_str[arg->i] == '\'')
		copy_in_single_quote(arena, arg, str);
	else
		copy_in_double_quote(arena, arg, str);
	arg->exist = true;
}
