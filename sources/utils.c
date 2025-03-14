/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 22:03:22 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/14 15:18:13 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"



static 
int	remove_quotes2(char *string, t_token *data)
{
	char quote;
	size_t i;
	int size;

	i = 0;
	size = 0;
	while (i < data->string_len)
	{
		if (data->string[i] == '"' || data->string[i] == '\'')
		{
			quote = data->string[i++];
			while (1)
			{
				if (data->string[i] == quote || data->string[i] == '\0')
				{
					size -= 1; // maybe this is just when hitting a quote?
					break ;
				}
			}
			string[size++] = data->string[i++];
		}
		string[size++] = data->string[i++];
	}
	return (size);
}

char *remove_quotes(t_arena *arena, t_token *data)
{
	char *string;
	int	new_size;

	string = arena_alloc(arena, sizeof(char) * data->string_len + 1);
	new_size = remove_quotes2(string, data);
	arena_unalloc(arena, data->string_len - new_size);
	return (string);
}
