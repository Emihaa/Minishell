/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 19:31:01 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/23 20:19:52 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// builtin exit somebody can add anything they want a exit code
// atol
// exit 100 = $? = 125
// do i write this out or printf it out or what?
int	small_itoa(t_expand_vars *v, char *str)
{
	int	value;
	int	size;

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

/// @brief append the current exit code to the str and increment index by two
/// @param arena used for allocations when necessary
/// @param arg to move the arg read index forward by two
/// @param str where to copy to
void	copy_exit_code(t_arena *arena, t_arg *arg, t_string *str)
{
	int		exit_code;
	int		len;
	int		i;
	char	buff[3];

	exit_code = get_minishell(NULL)->exit_status;
	len = num_len(exit_code);
	i = len;
	while (i--)
	{
		buff[i] = (exit_code % 10) + '0';
		exit_code /= 10;
	}
	append_to_string(arena, str, buff, len);
	arg->i += 2;
}
