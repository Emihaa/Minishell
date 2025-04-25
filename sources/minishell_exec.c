/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 03:52:05 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/25 21:31:14 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parser.h"

/// @todo error checking
int	create_and_store_pipe(t_minishell *m, int8_t *side)
{
	if (*side == WRITE || *side == -1)
	{
		if (pipe(m->pipe) == -1)
			syscall_failure(m, __FILE__, __LINE__);
		store_write_fd(m->pipe[WRITE], m);
		m->pipe[WRITE] = -1;
		*side = READ;
		return (0);
	}
	else if (*side == READ)
	{
		store_read_fd(m->pipe[READ], m);
		m->pipe[READ] = -1;
		*side = WRITE;
		return (0);
	}
	return (0);
}

void	close_heredocs(t_minishell *m)
{
	uint32_t	i;

	i = 0;
	while (i < m->heredoc_count)
	{
		if (m->heredoc_fds[i] != -1)
		{
			if (close(m->heredoc_fds[i]) == -1)
				syscall_failure(m, __FILE__, __LINE__);
		}
		i++;
	}
}

int	store_heredoc(t_minishell *m, int fd)
{
	uint32_t	i;

	i = 0;
	store_read_fd(fd, m);
	while (i < m->heredoc_count)
	{
		if (m->heredoc_fds[i] == fd)
		{
			m->heredoc_fds[i] = -1;
			break ;
		}
		i++;
	}
	return (0);
}

int	do_redir(t_minishell *m, t_token *data)
{
	int	status;

	status = 0;
	if (data->type > 0)
		status = store_heredoc(m, data->type);
	else if (data->type == REDIRECT_OUT)
		status = redirect_out(data->string, m);
	else if (data->type == REDIRECT_IN)
		status = redirect_in(data->string, m);
	else if (data->type == REDIRECT_APPEND)
		status = redirect_append(data->string, m);
	else if (data->type == REDIRECT_AMBI)
		status = redirect_ambi(data->string);
	return (status);
}

int	minishell_exec_loop(t_arena *arena, t_minishell *m, t_node *tree)
{
	t_node	*current_head;
	int		status;

	m->pipe_side = -1;
	status = 0;
	while (tree)
	{
		current_head = tree;
		if (m->pipe_side == READ)
			status = create_and_store_pipe(m, &m->pipe_side);
		if (tree->token.type == PIPE)
			status = create_and_store_pipe(m, &m->pipe_side);
		while (tree)
		{
			status = do_redir(m, &tree->token);
			if (tree->token.type == WORD || status != 0)
				if (execute_command(arena, m, tree->token.argv, status))
					break ;
			tree = tree->left;
		}
		reset_redirect(m);
		tree = current_head->right;
	}
	return (0);
}
