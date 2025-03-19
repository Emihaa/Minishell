/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 18:44:53 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/19 22:26:02 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int store_write_fd(int out_fd, t_minishell *minishell)
{
	if (minishell->redir_fds[WRITE] != STDOUT_FILENO)
		if (close(minishell->redir_fds[WRITE]) == -1)
			return (666); // @TODO: error cheking
	minishell->redir_fds[WRITE] = out_fd;
	return (0);
}

int store_read_fd(int in_fd, t_minishell *minishell)
{
	if (minishell->redir_fds[READ] != STDIN_FILENO)
		if (close(minishell->redir_fds[READ]) == -1)
			return (666); // @TODO: error cheking
	minishell->redir_fds[READ] = in_fd;
	return (0);
}

void apply_redirect(t_minishell *minishell)
{
	if (minishell->redir_fds[READ] != STDIN_FILENO)
	{
		if (dup2(minishell->redir_fds[READ], STDIN_FILENO) == -1)
			; // @TODO: error cheking
		if (close(minishell->redir_fds[READ]) == -1)
			; // @TODO: error cheking;
		minishell->redir_fds[READ] = STDIN_FILENO;
	}
	if (minishell->redir_fds[WRITE] != STDOUT_FILENO)
	{
		if (dup2(minishell->redir_fds[WRITE], STDOUT_FILENO) == -1)
			; // @TODO: error cheking
		if (close(minishell->redir_fds[WRITE]) == -1)
			; // @TODO: error cheking
		minishell->redir_fds[WRITE] = STDOUT_FILENO;
	}
}

void reset_redirect(t_minishell *minishell)
{
	if (minishell->redir_fds[READ] != STDIN_FILENO)
	{
		if (close(minishell->redir_fds[READ]) == -1)
			; // @TODO: error cheking
		minishell->redir_fds[READ] = STDIN_FILENO;
	}
	if (minishell->redir_fds[WRITE] != STDOUT_FILENO)
	{
		if (close(minishell->redir_fds[WRITE]) == -1)
			; // @TODO: error cheking
		minishell->redir_fds[WRITE] = STDOUT_FILENO;
	}
}

// prototyyype
// int redirect_out(t_arena *arena, t_token *data)
// {
// 	t_arena_temp temp;
// 	temp = arena_temp_begin
// }
