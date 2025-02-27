/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 17:34:59 by ltaalas           #+#    #+#             */
/*   Updated: 2025/02/24 17:36:30 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

// char *get_next_line(int fd)
// {
// 	static char buff[72000], *temp, *line;
// 	static int index;
// 	int bytes_read = 1, size = 0;

// 	if (index == 0)
// 		for (int i = 0; (bytes_read = read(fd, buff + i, BUFFER_SIZE)) > 0; i += bytes_read);
// 	if (bytes_read == -1 || buff[index] == '\0')
// 		return (NULL);
// 	temp = &buff[index];
// 	for (size = 0; temp[size] != '\n' && temp[size]; size++);
// 	line = malloc(++size + 1);
// 	line[size] = '\0';
// 	for (int i = 0; i < size; i++)
// 		line[i] = temp[i];
// 	index += size;
// 	return (line);
// }

char *get_next_line(int fd)
{
	static char buff[72000];
	static int index, bytes_read = 1, size;
	int i = 0;

	if (index == 0)
	{
		while (bytes_read > 0)
		{
			bytes_read = read(fd, buff + i, BUFFER_SIZE);
			i += bytes_read;
		}
	}
	if (bytes_read < 0 || buff[index] == '\0') 
		return (NULL);
	char *line = malloc(72000);
	size = 0;
	while (buff[index + size] != '\0')
	{
		line[size] = buff[index + size];
		if (buff[index + size] == '\n')
			break ;
		size++;
	}
	line[++size] = '\0';
	index += size;
	return (line);
}

#include <stdio.h>
#include <fcntl.h>
int main(int argc, char **argv)
{
	char *line;
	int fd = open("Makefile", O_RDONLY);
	while (1)
	{
		if ((line = get_next_line(fd)) == NULL)
			break;
		printf("%s", line);
		free(line);
	}
}
