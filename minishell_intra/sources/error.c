/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 17:24:03 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/26 18:55:03 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/// exit in an error situation
/// setting exit_status is optional, if set to 0 m->exit_status will stay as is
/// otherwise it is set to exit_status
void	error_exit(t_minishell *m, int exit_status)
{
	if (exit_status > 0)
		m->exit_status = exit_status;
	minishell_cleanup(m);
	exit(m->exit_status);
}

#ifdef MSHELL_DEBUG

void	syscall_failure(t_minishell *m, char *file, int line)
{
	stdout = stderr;
	printf("minishell: syscall failed with [%s]\n", strerror(errno));
	printf("(%s:%i)\n", file, line);
	if (m->pipe_side == -1)
		wait_for_sub_processes(m);
	m->exit_status = 1;
	printf("exit\n");
	error_exit(m, 0);
}
#else

void	syscall_failure(t_minishell *m, char *file, int line)
{
	(void)file;
	(void)line;
	stdout = stderr;
	printf("minishell: syscall failed with [%s]\n", strerror(errno));
	if (m->pipe_side == -1)
		wait_for_sub_processes(m);
	m->exit_status = 1;
	printf("exit\n");
	error_exit(m, 0);
}
#endif
