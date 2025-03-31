/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 00:07:00 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/01 00:08:30 by ltaalas          ###   ########.fr       */
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
	stdin = stderr;
	printf("minishell: %s: %s\n", cmd, strerror(errno));
	builtin_exit(m);
}

void command_not_found(t_minishell *m, char *cmd)
{
	stdout = stderr;
	printf("minishell: %s: command not found\n", cmd);
	m->exit_status = 127;
	builtin_exit(m);
}