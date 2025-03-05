/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:47:15 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/05 20:21:51 by ltaalas          ###   ########.fr       */
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
	//open("/tmp", O_TMPFILE, F);
}


// will probably want to only return the fd for the last heredoc
// @TODO: figure out the best way to handle multiple overwriting heredocs

// quick test consept for a blocking heredoc implementation
// would not break the pipe
void	heredoc_sub_process(int out_fd, char *delimiter)
{
	const int delimiter_len = ft_strlen(delimiter);
	char *line;
	printf("%i\n", getpid());
	t_arena arena = arena_new(DEFAULT_ARENA_CAPACITY);
	char *buff_base = arena_alloc(&arena, 0);
	size_t current_total = 0;
	while (1)
	{
		line = readline("> ");
		// @TODO: check for realine failure?
		if (ft_strncmp(line, delimiter, delimiter_len) == 0)
			break ;
		size_t line_len = ft_strlen(line);
		if (current_total + line_len > DEFAULT_ARENA_CAPACITY)
		{
			printf("WHAT THE F*** ARE YOU DOING??\n\
				Your heredoc size is trying to go above %i bytes", DEFAULT_ARENA_CAPACITY);
			break;
		}
		arena_realloc(&arena, &buff_base[current_total], line_len * sizeof(char));
		ft_memmove(&buff_base[current_total], line, line_len * sizeof(char));
		current_total += line_len;
		free(line);
	}
	free(line);
	arena_realloc(&arena, &buff_base[current_total], 1 * sizeof(char));
	write(out_fd, buff_base, ft_strlen(buff_base));
	write(out_fd, "\n", 1);
	if (close(out_fd) == -1)
		; //@TODO: error cheking;
	printf("here_doc_sub_process_exit\n");
	arena_delete(&arena);
	exit(0);
}

int heredoc2(char *delimiter, pid_t *heredoc_pid)
{
	int pipe_fds[2];
	pid_t pid;
	
	if (pipe(pipe_fds) == -1)
	; // @TODO error cheking
	
	pid = fork();
	if (pid == (pid_t)-1)
		; // @TODO: error cheking
	else if (pid == (pid_t)0)
	{
		if (close(pipe_fds[STDIN_FILENO]) == -1)
			; // @TODO: error cheking
		heredoc_sub_process(pipe_fds[STDOUT_FILENO], delimiter);
	}
	*heredoc_pid = pid;
	if (close(pipe_fds[STDOUT_FILENO]) == -1)
		; // @TODO: error cheking
	return (pipe_fds[STDIN_FILENO]);
}

//	Testing stuff
/*
cc heredoc.c -l readline ../libs/libft/build/libft.a ../libs/lt_alloc/build/lt_alloc.a -o heredoc

*/

int main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	char buf[1024] = {0};
	while (1)
	{
		read(STDIN_FILENO, buf, 1024);
		write(STDOUT_FILENO, buf, ft_strlen(buf));

	}
	exit(1);


	int wstatus;
	pid_t heredoc_pid;
	//read_loop(envp);
	pid_t pid = fork();
	printf("%i\n", getpid());
	if (pid == 0)
	{
		int in_fd = heredoc2("DELIM", &heredoc_pid);
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
		//pid = waitpid(heredoc_pid, &wstatus, 0);
		//printf("cat sub_process parent: 	%s\t%i\n", strerror(WEXITSTATUS(wstatus)), wstatus);
		char *cat_argv[2] = {[0] = "cat", [1] = NULL};
		execve("/bin/cat", cat_argv, envp);
		exit(1);
	}
	pid = waitpid(pid, &wstatus, 0);
	printf("%s\t%i\n", strerror(WEXITSTATUS(wstatus)), wstatus);
	printf("exit\n");
    return (0);
}


