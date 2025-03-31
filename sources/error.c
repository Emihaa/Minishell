/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 17:24:03 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/31 17:41:24 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	syscall_failure(t_minishell *m)
{
	stdout = stderr;
	printf("minishell: syscall failed with [%s]\n\
	Trying to wait for subprocesses and exiting\n", strerror(errno));
	// perror("Syscall failed. Trying to wait for subprocesses and exiting");
	wait_for_sub_processes(m);
	m->exit_status = 1;
	builtin_exit(m);
}
