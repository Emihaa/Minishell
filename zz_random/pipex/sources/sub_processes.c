/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sub_processes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 20:58:40 by ltaalas           #+#    #+#             */
/*   Updated: 2025/02/18 21:38:57 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	execute_command(char *cmd_str, char **envp)
{
	char	*cmd_with_path;
	char	**cmd_argv;

	cmd_with_path = NULL;
	cmd_argv = ft_split(cmd_str, ' ');
	if (cmd_argv == NULL)
		error_malloc(NULL);
	cmd_with_path = check_for_cmd_path(cmd_str, cmd_argv, envp);
	if (is_directory(cmd_with_path))
		error_access(cmd_argv, cmd_with_path);
	if (access(cmd_with_path, X_OK))
		error_access(cmd_argv, cmd_with_path);
	execve(cmd_with_path, cmd_argv, envp);
	error_execve(cmd_argv, cmd_with_path);
}

void	first_child(int pipe[2], char *cmd_str, char *infile, char **envp)
{
	int		in_fd;

	in_fd = open(infile, O_RDONLY);
	if (in_fd < 0)
		error_open(pipe, infile);
	if (dup2(in_fd, STDIN_FILENO) == -1)
		perror(NULL);
	if (dup2(pipe[WRITE], STDOUT_FILENO) == -1)
		perror(NULL);
	close_fds((int [3]){pipe[READ], in_fd, pipe[WRITE]}, 3);
	execute_command(cmd_str, envp);
}

void	last_child(int pipe[2], char *cmd_str, char *outfile, char **envp)
{
	int		out_fd;

	out_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (out_fd < 0)
		error_open(pipe, outfile);
	if (dup2(out_fd, STDOUT_FILENO) == -1)
		perror(NULL);
	if (dup2(pipe[READ], STDIN_FILENO) == -1)
		perror(NULL);
	close_fds((int [3]){pipe[READ], out_fd, pipe[WRITE]}, 3);
	execute_command(cmd_str, envp);
}
