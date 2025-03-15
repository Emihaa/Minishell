/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 22:03:22 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/14 18:55:55 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
 
uint32_t	get_quote_removed_string(char *string, t_token *data)
{
	char quote;
	size_t data_index;
	size_t string_index;

	data_index = 0;
	string_index = 0;
	while (data_index < data->string_len)
	{
		if (data->string[data_index] == '"' || data->string[data_index] == '\'')
		{
			quote = data->string[data_index++];
			while (data_index < data->string_len)
			{
				if (data->string[data_index] == quote)
				{
					data_index += 1;
					break ;
				}
				string[string_index++] = data->string[data_index++];
			}
			continue ;
		}
		string[string_index++] = data->string[data_index++];
	}
	return (string_index);
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

ssize_t write_cheking_thingy_asd(int fd, char *str, size_t str_len)
{
	size_t bytes_to_write;
	ssize_t bytes_written;

	bytes_to_write = str_len;
	bytes_written = 0;
	while (bytes_to_write > 0)
	{
		bytes_written = write(fd, str + bytes_written, bytes_to_write);
		if (bytes_written == -1)
			return (-1); // full error situation
		bytes_to_write -= bytes_written;
	}
	return (0);
}
