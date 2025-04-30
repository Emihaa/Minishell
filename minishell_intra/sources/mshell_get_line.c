/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mshell_get_line.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 23:18:07 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/30 19:44:46 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static
void	read_failure(void)
{
	t_minishell	*m;

	m = get_minishell(NULL);
	stdout = stderr;
	printf("minishell: line %i: read failure: %s\n",
		m->line_counter, strerror(errno));
	m->exit_status = 1;
}

static
int	read_file_to_buff(t_arena *arena, char **buffer, int fd)
{
	ssize_t	bytes_read;
	size_t	i;
	size_t	allocated;

	i = 0;
	allocated = 4096;
	*buffer = xarena_alloc(arena, sizeof(char) * (allocated + 1));
	bytes_read = 1;
	while (bytes_read > 0)
	{
		if (i >= allocated)
		{
			*buffer = xarena_realloc(arena, *buffer, i, (allocated * 2) + 1);
			allocated *= 2;
		}
		bytes_read = read(fd, (*buffer) + i, 1024);
		i += bytes_read;
	}
	if (bytes_read >= 0)
		return (0);
	read_failure();
	return (-1);
}

char	*get_line(t_arena *arena, int fd)
{
	static size_t	read_head = 0;
	static char		*buff;
	char			*line;
	size_t			i;

	line = NULL;
	if (read_head <= 0)
	{
		if (read_file_to_buff(arena, &buff, fd))
			return (NULL);
	}
	if (buff[read_head] == '\0')
		return (NULL);
	i = 0;
	while (buff[read_head + i] != '\n' && buff[read_head + i] != '\0')
		i += 1;
	line = ft_substr(buff, read_head, i);
	read_head += i + 1;
	if (line == NULL)
	{
		(void)put_str(2, "minishell: Allocation failure\n");
	}
	return (line);
}
