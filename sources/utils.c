/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 22:03:22 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/02 22:57:54 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

uint8_t	num_len(uint32_t num)
{
	uint8_t i;

	i = 0;
	if (num == 0)
		return (1);
	while (num > 0)
	{	
		num = num / 10;
		i++;
	}
	return (i);
}
inline
bool	is_space(char c)
{
	return (c == ' ' || c == '\t'); // || c == '\n' maybe?
}

t_minishell *get_minishell(t_minishell *m)
{
	static t_minishell *minishell;

	if (m == NULL)
		return (minishell);
	minishell = m;
	return (NULL);
}
void *xarena_alloc(t_arena *arena, uint64_t size)
{
	void *value;
	
	value = arena_alloc(arena, size);
	if (value == NULL)
	{
		put_str_nl(2, "allocation failure");
		error_exit(get_minishell(NULL));
	}
	return (value);
}

void *xarena_alloc_no_zero(t_arena *arena, uint64_t size)
{
	void *value;
	
	value = arena_alloc_no_zero(arena, size);
	if (value == NULL)
	{
		put_str_nl(2, "allocation failure");
		error_exit(get_minishell(NULL));
	}
	return (value);
}
