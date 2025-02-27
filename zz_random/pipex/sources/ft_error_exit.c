/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 22:27:10 by ltaalas           #+#    #+#             */
/*   Updated: 2025/02/16 00:12:34 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	error_execve(char **cmd_argv, char *cmd_with_path)
{
	perror(cmd_with_path);
	if (cmd_with_path != cmd_argv[0])
	{
		free(cmd_with_path);
		cmd_with_path = NULL;
	}
	if (cmd_argv != NULL)
		free_str_arr(cmd_argv);
	exit (EXIT_FAILURE);
}

void	error_open(int *pipe, char *file)
{
	perror(file);
	if (pipe != NULL)
	{
		close(pipe[0]);
		close(pipe[1]);
	}
	exit(EXIT_FAILURE);
}

void	error_malloc(char **cmd_argv)
{
	perror(NULL);
	if (cmd_argv != NULL)
		free_str_arr(cmd_argv);
	exit(EXIT_FAILURE);
}

void	error_access(char **cmd_argv, char *cmd_with_path)
{
	perror(cmd_with_path);
	if (cmd_with_path != cmd_argv[0])
	{
		free(cmd_with_path);
		cmd_with_path = NULL;
	}
	if (cmd_argv != NULL)
		free_str_arr(cmd_argv);
	if (errno == EACCES || errno == EISDIR || errno == ENOTDIR)
		exit(126);
	if (errno == ENOENT)
		exit(127);
	exit(EXIT_FAILURE);
}

void	error_command_not_found(char **cmd_argv, char *cmd_with_path,
								char *to_print)
{
	ft_putstr_fd(to_print, 2);
	ft_putendl_fd(": Command not found", 2);
	if (cmd_with_path != cmd_argv[0])
	{
		free(cmd_with_path);
		cmd_with_path = NULL;
	}
	if (cmd_argv != NULL)
		free_str_arr(cmd_argv);
	exit(127);
}
