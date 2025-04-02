/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 22:56:53 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/02 23:04:05 by ltaalas          ###   ########.fr       */
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

// @TODO: change to name to /tmp/...
// we need some kind of global heredoc count to check if it is under 17
char	*create_temp_file_name(uint32_t heredoc_num)
{
	//static uint32_t heredoc_num = 1; // this should probably be included into the minishell struct and passed here
									// also should be reset whenever starting a new command reading loop
	static char name_buf[30] = HEREDOC_TEMP_NAME;
	uint32_t num_temp;
	uint8_t i;

	num_temp = heredoc_num;
	i = num_len(heredoc_num);
	name_buf[NAME_BASE_LEN + i] = '\0';
	while(i--)
	{
		name_buf[NAME_BASE_LEN + i] = (num_temp % 10) + '0';
		num_temp = num_temp / 10;
	}
	heredoc_num += 1;
	return(name_buf);
}

// @TODO: change to name to /tmp
// @TODO: might want to make different error return values
int	create_heredoc_fds(int fds[2])
{
	static uint32_t heredoc_num = 1;
	const char *file_name;
	int return_val;

	return_val = 0;
	heredoc_num = 1;
	while (1)
	{
		file_name = create_temp_file_name(heredoc_num++);
		fds[1] = open(file_name, O_EXCL | O_CREAT | O_CLOEXEC | O_WRONLY, S_IWUSR | S_IRUSR); 
		if (fds[1] != -1)
			break ;
		if (errno == EEXIST)
			continue ;
		return (-1); // @TODO: more error stuff
	}	
	fds[0] = open(file_name, O_CLOEXEC | O_RDONLY);
	if (fds[0] == -1)
		return_val = -1;
	if (unlink(file_name) == -1)
		return_val = -1; // @TODO: more error stuff
	return (return_val);
}
