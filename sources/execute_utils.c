/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 00:07:00 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/20 01:26:19 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void close_pipe(t_minishell *m)
{
	if (m->pipe[WRITE] != -1)
		if (close(m->pipe[WRITE]))
			syscall_failure(m);
	if (m->pipe[READ] != -1)
		if (close(m->pipe[READ]))
			syscall_failure(m);
}
// maybe just add this to run commmand
void execve_failure(t_minishell *m, char *cmd)
{
	m->exit_status = 1;
	if (errno == ENOENT)
		m->exit_status = 127;
	if (errno == EACCES)
		m->exit_status = 126;
	if (errno == EISDIR || errno == ENOTDIR)
		m->exit_status = 126;
	stdout = stderr;
	if (m->istty)
		printf("minishell: %s: %s\n", cmd, strerror(errno));
	else
		printf("minishell: line %i: %s: %s\n", m->line_counter, cmd, strerror(errno));
	error_exit(m, -1);
}

void command_not_found(t_minishell *m, char *cmd)
{
	stdout = stderr;
	if (m->istty)
		printf("minishell: %s: command not found\n", cmd);
	else
		printf("minishell: line %i: %s: command not found\n", m->line_counter, cmd);
	m->exit_status = 127;
	error_exit(m, -1);
}