/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 00:07:00 by ltaalas           #+#    #+#             */
/*   Updated: 2025/05/08 00:26:29 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/builtin.h"
#include <sys/stat.h>

void	close_pipe(t_minishell *m)
{
	if (m->pipe[WRITE] != -1)
	{
		if (close(m->pipe[WRITE]))
		{
			m->pipe[WRITE] = -1;
			syscall_failure(m, __FILE__, __LINE__);
		}
		m->pipe[WRITE] = -1;
	}
	if (m->pipe[READ] != -1)
	{
		if (close(m->pipe[READ]))
		{
			m->pipe[READ] = -1;
			syscall_failure(m, __FILE__, __LINE__);
		}
		m->pipe[READ] = -1;
	}
}

void	execute_builtin_in_main_process(t_minishell *m,
		char **argv, int builtin_type)
{
	if (m->redir_fds[WRITE] != STDOUT_FILENO)
	{
		m->stdout_dup = dup(STDOUT_FILENO);
		if (m->stdout_dup == -1)
			syscall_failure(m, __FILE__, __LINE__);
		if (dup2(m->redir_fds[WRITE], STDOUT_FILENO) == -1)
			syscall_failure(m, __FILE__, __LINE__);
		if (close(m->redir_fds[WRITE]))
			syscall_failure(m, __FILE__, __LINE__);
	}
	execute_builtin(m, argv, builtin_type);
	if (m->redir_fds[WRITE] != STDOUT_FILENO)
	{
		m->redir_fds[WRITE] = STDOUT_FILENO;
		if (dup2(m->stdout_dup, STDOUT_FILENO) == -1)
			syscall_failure(m, __FILE__, __LINE__);
		if (close(m->stdout_dup))
			syscall_failure(m, __FILE__, __LINE__);
		m->stdout_dup = -1;
	}
}

int	directory_check(char *cmd)
{
	int			errno_temp;
	struct stat	sb;

	errno_temp = errno;
	if (stat(cmd, &sb) == 0)
	{
		if (S_ISDIR(sb.st_mode))
		{
			return (1);
		}
	}
	errno = errno_temp;
	return (0);
}

// maybe just add this to run commmand
void	execve_failure(t_minishell *m, char *cmd)
{
	FILE	*temp;

	temp = stdout;
	m->exit_status = 1;
	if (errno == ENOENT)
		m->exit_status = 127;
	else if (errno == EACCES || errno == EISDIR || errno == ENOTDIR)
	{
		m->exit_status = 126;
		if (directory_check(cmd))
			errno = EISDIR;
	}
	stdout = stderr;
	if (m->istty)
		printf("minishell: %s: %s\n", cmd, strerror(errno));
	else
		printf("minishell: line %i: %s: %s\n", m->line_counter,
			cmd, strerror(errno));
	stdout = temp;
	error_exit(m, -1);
}

void	command_not_found(t_minishell *m, char *cmd)
{
	FILE	*temp;

	temp = stdout;
	stdout = stderr;
	if (m->istty)
		printf("minishell: %s: command not found\n", cmd);
	else
		printf("minishell: line %i: %s: command not found\n",
			m->line_counter, cmd);
	m->exit_status = 127;
	stdout = temp;
	error_exit(m, -1);
}
