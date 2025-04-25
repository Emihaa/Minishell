/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 17:39:28 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/25 01:53:01 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/parser.h"

// @TODO: restructure error messages
// we might want to do some kind of printf thing with a buffer
int	redirect_ambi(char *file_name)
{
	stdout = stderr;
	printf("minishell: %s: ambigous redirect\n", file_name);
	return (1);
}

static inline
void	open_failure(char *file_name)
{
	stdout = stderr;
	printf("minishell: %s: %s\n", file_name, strerror(errno));
}

int	redirect_out(char *file_name, t_minishell *m)
{
	int	fd;

	fd = open(file_name, O_CLOEXEC | O_CREAT | O_WRONLY | O_TRUNC,
			S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
	if (fd == -1)
	{
		open_failure(file_name);
		return (1);
	}
	store_write_fd(fd, m);
	return (0);
}

int	redirect_append(char *file_name, t_minishell *m)
{
	int	fd;

	fd = open(file_name, O_CLOEXEC | O_CREAT | O_WRONLY | O_APPEND,
			S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
	if (fd == -1)
	{
		open_failure(file_name);
		return (1);
	}
	store_write_fd(fd, m);
	return (0);
}

int	redirect_in(char *file_name, t_minishell *m)
{
	int	fd;

	fd = open(file_name, O_CLOEXEC | O_RDONLY);
	if (fd == -1)
	{
		open_failure(file_name);
		return (1);
	}
	store_read_fd(fd, m);
	return (0);
}

int	redirect_heredoc(t_token *data, t_minishell *m)
{
	const int	fd = data->type;

	store_read_fd(fd, m);
	m->heredoc_fds[m->heredoc_count] = -1;
	return (0);
}
