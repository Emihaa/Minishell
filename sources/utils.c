/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 22:03:22 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/18 19:53:11 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


// give an allocated dest pointer that is atleast the size of {data->string_len + 1}
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
				dest[dest_index++] = data->string[data_index++];
			}
			continue ;
		}
		dest[dest_index++] = data->string[data_index++];
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
