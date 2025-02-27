/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 20:40:04 by ltaalas           #+#    #+#             */
/*   Updated: 2025/02/18 21:01:03 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

static void	wait_for_sub_processes(int command_count, pid_t last_command_pid)
{
	int		i;
	int		wstatus;
	int		exit_code;
	pid_t	pid;

	i = 0;
	exit_code = 1;
	while (i < command_count)
	{
		pid = wait(&wstatus);
		if (pid == (pid_t)(-1))
			perror(NULL);
		if (pid == last_command_pid)
		{
			exit_code = WEXITSTATUS(wstatus);
		}
		i++;
	}
	exit(exit_code);
}

int	main(int argc, char *argv[], char *envp[])
{
	int		pipe_fd[2];
	int		fork_count;
	pid_t	pid;

	fork_count = 0;
	if (argc != 5)
		invalid_argument_count();
	if (pipe(pipe_fd) < 0)
		error_open(NULL, "Pipe creation failed");
	pid = fork();
	if (pid < (pid_t)0)
		perror("Fork failure");
	else if (pid == (pid_t)0)
		first_child(pipe_fd, argv[2], argv[1], envp);
	else
		fork_count++;
	pid = fork();
	if (pid < (pid_t)0)
		perror("Fork failure");
	else if (pid == (pid_t)0)
		last_child(pipe_fd, argv[argc - 2], argv[argc - 1], envp);
	else
		fork_count++;
	close_fds((int [2]){pipe_fd[0], pipe_fd[1]}, 2);
	wait_for_sub_processes(fork_count, pid);
}
