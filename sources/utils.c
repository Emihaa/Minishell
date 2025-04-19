/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 22:03:22 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/17 23:58:15 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

inline
uint32_t	eat_space(char *str)
{
	uint32_t	i;

	i = 0;
	while (is_space(str[i]))
		i++;
	return (i);
}

uint8_t	num_len(uint32_t num)
{
	uint8_t	i;

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

// || c == '\n' maybe?
inline
bool	is_space(char c)
{
	return (c == ' ' || c == '\t');
}

t_minishell	*get_minishell(t_minishell *m)
{
	static t_minishell	*minishell;

	if (m == NULL)
		return (minishell);
	minishell = m;
	return (NULL);
}
