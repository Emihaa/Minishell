/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 19:19:56 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/17 23:18:18 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	write_bytes(int fd, char *str, size_t bytes_to_write)
{
	ssize_t bytes_written;

	bytes_written = 0;
	while (bytes_to_write > 0)
	{
		bytes_written = write(fd, str, bytes_to_write);
		if (bytes_written == -1)
			return (-1); // full error situation
		str += bytes_written;
		bytes_to_write -= bytes_written;
	}
	return (0);
}

int	put_str_nl(int fd, char *str)
{
	int r_val;

	r_val = 0;
	if (str != NULL)
	{
		r_val = write_bytes(fd, str, ft_strlen(str));
	}
	write_bytes(fd, "\n", 1);
	return (r_val);
}

int	put_str(int fd, char *str)
{
	int r_val;

	r_val = 0;
	if (str != NULL)
	{
		r_val = write_bytes(fd, str, ft_strlen(str));
	}
	return (r_val);
}

inline
int	put_char(int fd, char c)
{
	return (write_bytes(fd, &c, 1));
}

