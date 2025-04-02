/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 22:03:22 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/02 20:30:25 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


// give an allocated dest pointer that is atleast the size of {data->u_data.string_len + 1}
// will return the new size of the string that has quotes removed
// the 
uint32_t	set_quote_removed_string(char *dest, t_token *data)
{
	char quote;
	uint32_t data_index; // feel like this or these should be renamde
	uint32_t dest_index;

	data_index = 0; 
	dest_index = 0;
	while (data_index < data->string_len)
	{
		if (data->u_data.string[data_index] == '"' || data->u_data.string[data_index] == '\'')
		{
			quote = data->u_data.string[data_index++];
			while (data_index < data->string_len)
			{
				if (data->u_data.string[data_index] == quote)
				{
					data_index += 1;
					break ;
				}
				dest[dest_index++] = data->u_data.string[data_index++];
			}
			continue ;
		}
		dest[dest_index++] = data->u_data.string[data_index++];
	}
	return (dest_index);
}

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
