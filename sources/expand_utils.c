/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 19:31:01 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/18 22:05:33 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// builtin exit somebody can add anything they want a exit code
// atol
// exit 100 = $? = 125
// do i write this out or printf it out or what?
int small_itoa(t_expand_vars *v, char *str)
{
	int value;
	int size;

	v->i += 2;
	size = 0;
	value = get_minishell(NULL)->exit_status;
	if (value == 0)
	{
		str[v->len++] = '0';
		return (0);
	}
	size = (int) num_len((uint32_t) value);
	str += v->len;
	v->len += size;
	while (value > 0)
	{
		str[--size] = value % 10 + '0';
		value /= 10;
	}
	return (0);
}

void	copy_exit_code(t_arena *arena, t_arg *arg, t_string *str)
{
	int exit_code;
	int	len;
	int i;
	char buff[3];

	exit_code = get_minishell(NULL)->exit_status;
	len = num_len(exit_code);
	i = len;
	while(i--)
	{
		buff[i] = (exit_code % 10) + '0';
		exit_code /= 10;
	}
	append_to_string(arena, str, buff, len);
	arg->i += 2;
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

void	copy_until_quote_or_var(t_arena *arena, t_arg *arg, t_string *str)
{
	uint32_t len;

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

void expand_variable_in_quotes(t_arena *arena, t_arg *arg, t_string *str)
{
	if (arg->data_str[arg->i + 1] == '?')
	{
		copy_exit_code(arena, arg, str);

	}
	else if (is_valid_var_start(arg->data_str[arg->i + 1]))
	{
		arg->i += 1;
		copy_full_env_var(arena, arg, str);
	}
	else
	{
		append_to_string(arena, str, &arg->data_str[arg->i], 1);
		arg->i += 1;
	}
}
