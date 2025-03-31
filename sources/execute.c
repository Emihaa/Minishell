/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 21:05:55 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/31 21:06:29 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

pid_t	execute_subprocess(t_minishell *m, char **argv, t_builtin builtin)
{
	pid_t	pid;

	for (int i = 0; argv[i] != NULL; ++i)
	{
		printf("i: %i\n", i);
		printf("argv[%i]: %s\n", i, argv[i]);
	}
	pid = fork();
	if (pid == (pid_t)(-1))
		syscall_failure(m); // @TODO: error cheking
	if (pid == 0)
	{
		apply_redirect(m);
		close_pipe(m);
		if (builtin != BUILTIN_FALSE)
		{
			execute_builtin(m, argv, builtin);
			exit(m->exit_status);
		}
		run_command(m, argv);
	}
	m->command_count += 1;
	return (pid);
}

void execute_command(t_minishell *m, char **argv, int status)
{
	pid_t pid;
	t_builtin builtin_type;

	if (argv == NULL)
		return ;
	if (status != 0)
	{
		pid = fork();
		if (pid == -1)
			syscall_failure(m); // @TODO: error cheking
		if (pid == 0)
			error_exit(m, status);
		m->command_count += 1;
		m->last_pid = pid;
		return ;
	}
	builtin_type = check_for_builtin(argv[0]);
	if (m->pipe_side == -1 && builtin_type != BUILTIN_FALSE)
	{
		execute_builtin(m, argv, builtin_type);
		return ;
	}
	m->last_pid = execute_subprocess(m, argv, builtin_type);
}