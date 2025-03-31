/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 18:44:53 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/28 16:38:39 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// static inline
void	syscall_failure(t_minishell *m)
{
	stdout = stderr;
	printf("minishell: syscall failed with [%s]\n\
	Trying to wait for subprocesses and exiting\n", strerror(errno));
	// perror("Syscall failed. Trying to wait for subprocesses and exiting");
	wait_for_sub_processes(m);
	error_exit(m, 1);
}

void	store_write_fd(int out_fd, t_minishell *minishell)
{
	if (minishell->redir_fds[WRITE] != STDOUT_FILENO)
		if (close(minishell->redir_fds[WRITE]) == -1)
			syscall_failure(minishell); // @TODO: error checking
	minishell->redir_fds[WRITE] = out_fd;
}

void	store_read_fd(int in_fd, t_minishell *minishell)
{
	if (minishell->redir_fds[READ] != STDIN_FILENO)
		if (close(minishell->redir_fds[READ]) == -1)
			syscall_failure(minishell); // @TODO: error checking
	minishell->redir_fds[READ] = in_fd;
}

void	apply_redirect(t_minishell *minishell)
{
	if (minishell->redir_fds[READ] != STDIN_FILENO)
	{
		if (dup2(minishell->redir_fds[READ], STDIN_FILENO) == -1)
			syscall_failure(minishell); // @TODO: error checking
		if (close(minishell->redir_fds[READ]) == -1)
			syscall_failure(minishell); // @TODO: error checking
		minishell->redir_fds[READ] = STDIN_FILENO;
	}
	if (minishell->redir_fds[WRITE] != STDOUT_FILENO)
	{
		if (dup2(minishell->redir_fds[WRITE], STDOUT_FILENO) == -1)
			syscall_failure(minishell); // @TODO: error checking
		if (close(minishell->redir_fds[WRITE]) == -1)
			syscall_failure(minishell); // @TODO: error checking
		minishell->redir_fds[WRITE] = STDOUT_FILENO;
	}
}

void	reset_redirect(t_minishell *minishell)
{
	if (minishell->redir_fds[READ] != STDIN_FILENO)
	{
		if (close(minishell->redir_fds[READ]) == -1)
			syscall_failure(minishell); // @TODO: error cheking
		minishell->redir_fds[READ] = STDIN_FILENO;
	}
	if (minishell->redir_fds[WRITE] != STDOUT_FILENO)
	{
		if (close(minishell->redir_fds[WRITE]) == -1)
			syscall_failure(minishell); // @TODO: error cheking
		minishell->redir_fds[WRITE] = STDOUT_FILENO;
	}
}

// prototyyype
// int redirect_out(t_arena *arena, t_token *data)
// {
// 	t_arena_temp temp;
// 	temp = arena_temp_begin
// }
