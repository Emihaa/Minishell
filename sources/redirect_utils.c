/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 18:44:53 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/25 20:33:00 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	store_write_fd(int out_fd, t_minishell *minishell)
{
	if (minishell->redir_fds[WRITE] != STDOUT_FILENO)
		if (close(minishell->redir_fds[WRITE]) == -1)
			syscall_failure(minishell, __FILE__, __LINE__); // @TODO: error checking
	minishell->redir_fds[WRITE] = out_fd;
}

void	store_read_fd(int in_fd, t_minishell *minishell)
{
	if (minishell->redir_fds[READ] != STDIN_FILENO)
		if (close(minishell->redir_fds[READ]) == -1)
			syscall_failure(minishell, __FILE__, __LINE__); // @TODO: error checking
	minishell->redir_fds[READ] = in_fd;
}

void	apply_redirect(t_minishell *minishell)
{
	if (minishell->redir_fds[READ] != STDIN_FILENO)
	{
		if (dup2(minishell->redir_fds[READ], STDIN_FILENO) == -1)
			syscall_failure(minishell, __FILE__, __LINE__); // @TODO: error checking
		if (close(minishell->redir_fds[READ]) == -1)
			syscall_failure(minishell, __FILE__, __LINE__); // @TODO: error checking
		minishell->redir_fds[READ] = STDIN_FILENO;
	}
	if (minishell->redir_fds[WRITE] != STDOUT_FILENO)
	{
		if (dup2(minishell->redir_fds[WRITE], STDOUT_FILENO) == -1)
			syscall_failure(minishell, __FILE__, __LINE__); // @TODO: error checking
		if (close(minishell->redir_fds[WRITE]) == -1)
			syscall_failure(minishell, __FILE__, __LINE__); // @TODO: error checking
		minishell->redir_fds[WRITE] = STDOUT_FILENO;
	}
}

void	reset_redirect(t_minishell *minishell)
{
	if (minishell->redir_fds[READ] != STDIN_FILENO)
	{
		if (close(minishell->redir_fds[READ]) == -1)
			syscall_failure(minishell, __FILE__, __LINE__); // @TODO: error cheking
		minishell->redir_fds[READ] = STDIN_FILENO;
	}
	if (minishell->redir_fds[WRITE] != STDOUT_FILENO)
	{
		if (close(minishell->redir_fds[WRITE]) == -1)
			syscall_failure(minishell, __FILE__, __LINE__); // @TODO: error cheking
		minishell->redir_fds[WRITE] = STDOUT_FILENO;
	}
}
