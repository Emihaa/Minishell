/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 14:55:36 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/23 20:22:32 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

static char	*ft_join_line(char *old_line, char *buffer, size_t i)
{
	char	*tmp;
	char	*new_line;

	tmp = NULL;
	new_line = NULL;
	tmp = ft_substr(buffer, 0, i);
	if (tmp == NULL)
		return (NULL);
	new_line = ft_strjoin(old_line, tmp);
	free(tmp);
	free(old_line);
	if (new_line == NULL)
		return (NULL);
	return (new_line);
}

static void	ft_clean_buffer(char *buffer, size_t i, int status,
							char **return_line)
{
	size_t	j;

	j = 0;
	while (buffer[i] != '\0')
		buffer[j++] = buffer[i++];
	while (j < i)
		buffer[j++] = '\0';
	if (status < 0)
	{
		free(*return_line);
		*return_line = NULL;
	}
}

static char	*ft_build_line(char *return_line, char *buffer)
{
	size_t	i;

	i = 0;
	while (buffer[i] != '\0' && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\n')
		i++;
	if (return_line == NULL)
	{
		return_line = ft_substr(buffer, 0, i);
		if (return_line == NULL)
			return (NULL);
	}
	else
	{
		return_line = ft_join_line(return_line, buffer, i);
		if (return_line == NULL)
			return (NULL);
	}
	ft_clean_buffer(buffer, i, 42, &return_line);
	return (return_line);
}

static ssize_t	check_buffer(int fd, char *buffer)
{
	ssize_t	bytes_read;

	bytes_read = 1;
	if (*buffer == '\0')
		bytes_read = read(fd, buffer, BUFFER_SIZE);
	return (bytes_read);
}

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	char		*return_line;
	ssize_t		bytes_read;

	return_line = NULL;
	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	bytes_read = check_buffer(fd, buffer);
	if (bytes_read <= 0)
		return (NULL);
	while (bytes_read > 0)
	{
		return_line = ft_build_line(return_line, buffer);
		if (return_line == NULL)
			return (NULL);
		if (ft_strchr(return_line, '\n'))
			break ;
		bytes_read = read(fd, buffer, BUFFER_SIZE);
	}
	if (bytes_read < 0)
		ft_clean_buffer(buffer, 0, -42, &return_line);
	return (return_line);
}
