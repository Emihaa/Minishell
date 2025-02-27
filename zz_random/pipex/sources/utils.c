/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 22:31:13 by ltaalas           #+#    #+#             */
/*   Updated: 2025/02/23 17:47:25 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	invalid_argument_count(void)
{
	ft_putendl_fd("Invalid argument count", 2);
	ft_putendl_fd("Usage: [infile] {cmd1} {cmd2} [outfile]", 2);
	exit (1);
}

char	*free_str_arr(char **split)
{
	int	i;

	i = 0;
	if (split && *split)
	{
		while (split[i])
		{
			free(split[i]);
			split[i] = NULL;
			i++;
		}
	}
	if (split)
	{
		free(split);
		split = NULL;
	}
	return (NULL);
}

int	is_directory(char *cmd_with_path)
{
	int	fd;

	fd = open(cmd_with_path, O_DIRECTORY);
	if (fd != -1)
	{
		if (close(fd) < 0)
			perror(NULL);
		errno = EISDIR;
		return (1);
	}
	return (0);
}

// function to close many fds.
// currently only meant to be used for piping,
// so the fds array has enough space for,
// 2 pipes + 2 redirection fds.
// PLEASE give a correct count!
void	close_fds(int *fds, int count)
{
	int	i;
	int	error;

	i = 0;
	error = 0;
	while (i < count)
	{
		if (close(fds[i]) < 0)
		{
			error = 1;
			perror(NULL);
		}
		i++;
	}
	if (error == 1)
		exit(1);
}
