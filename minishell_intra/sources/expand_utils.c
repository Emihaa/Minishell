/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 19:31:01 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/26 19:05:01 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/expand.h"

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
