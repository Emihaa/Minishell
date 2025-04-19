/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 17:24:03 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/19 20:39:14 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// seems to be unused currently


// exit in an error situation
// setting exit_status is optional, if set to 0 m->exit_status will stay as is
// otherwise it is set to exit_status
void error_exit(t_minishell *m, int exit_status)
{
	if (exit_status > 0)
		m->exit_status = exit_status;
	minishell_cleanup(m);
	exit(m->exit_status);
}

void	syscall_failure(t_minishell *m)
{
	stdout = stderr;
	printf("minishell: syscall failed with [%s]\n\
	Trying to wait for subprocesses and exiting\n", strerror(errno));
	// perror("Syscall failed. Trying to wait for subprocesses and exiting");
	wait_for_sub_processes(m);
	m->exit_status = 1;
	printf("exit\n");
	error_exit(m, 0);
}
