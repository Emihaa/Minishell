/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 22:56:53 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/25 01:21:56 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/heredoc.h"

// give an allocated dest pointer that is atleast the size of
// {data->u_data.string_len + 1}
// will return the new size of the string that has quotes removed
// the 
uint32_t	set_quote_removed_string(char *dest, t_token *data)
{
	char		quote;
	uint32_t	data_index;
	uint32_t	dest_index;
	// feel like this or these should be renamde
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

// @TODO: change to name to /tmp/...
// we need some kind of global heredoc count to check if it is under 17
char	*create_temp_file_name(uint32_t heredoc_num)
{
	static char	name_buf[30] = HEREDOC_TEMP_NAME;
	uint32_t	num_temp;
	uint8_t		i;

	num_temp = heredoc_num;
	i = num_len(heredoc_num);
	name_buf[NAME_BASE_LEN + i] = '\0';
	while (i--)
	{
		name_buf[NAME_BASE_LEN + i] = (num_temp % 10) + '0';
		num_temp = num_temp / 10;
	}
	heredoc_num += 1;
	return (name_buf);
}

// @TODO: change to name to /tmp
// @TODO: might want to make different error return values
int	create_heredoc_fds(int fds[2])
{
	uint32_t	heredoc_num;
	char		*file_name;
	int			return_val;

	return_val = 0;
	heredoc_num = 1;
	while (1)
	{
		file_name = create_temp_file_name(heredoc_num++);
		fds[1] = open(
				file_name,
				O_EXCL | O_CREAT | O_CLOEXEC | O_WRONLY,
				S_IWUSR | S_IRUSR);
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

void	print_eof_error(t_minishell *m, char *delimiter)
{
	FILE	*temp;

	temp = stdout;
	stdout = stderr;
	printf(EOF_ERROR, m->line_counter, delimiter);
	stdout = temp;
}

int	heredoc_read(t_minishell *minishell, char **line, char *delimiter)
{
	if (minishell->istty == 1)
		rl_event_hook = heredoc_event_hook;
	*line = readline("> ");
	if (*line == NULL || g_int == SIGINT) // do this for other signals maybe
	{
		if (g_int == SIGINT)
		{
			minishell->exit_status = SIGINT + 128;
			g_int = 0;
			return (-2);
		}
		if (!*line)
			print_eof_error(minishell, delimiter);
		return (-1);
	}
	minishell->line_counter += 1;
	return (0);
}
