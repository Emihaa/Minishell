/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:47:15 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/11 17:54:11 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// this should be enough, will blow up the pipe!
// maybe create a pipe in parent make a fork for writing to pipe and one for the command to read from.

int	heredoc(char *delimiter)
{
	const int delimiter_len = ft_strlen(delimiter);
	char *line;
	int pipe_fds[2];

	if (pipe(pipe_fds) == -1)
		; // @TODO error cheking
	while (1)
	{
		line = readline("> ");
		// @TODO: check for realine failure?
		if (ft_strncmp(line, delimiter, delimiter_len))
		{
			free(line);
			break ;
		}
		write(pipe_fds[STDOUT_FILENO], line, ft_strlen(line));
		write(pipe_fds[STDOUT_FILENO], "\n", 1);
		free(line);
	}
	return (pipe_fds[STDIN_FILENO]);
}