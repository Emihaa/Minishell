/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 21:05:55 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/09 21:27:44 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *get_cmd_with_path(t_arena *a, t_minishell *m, char *cmd)
{
	const uint32_t	cmd_str_len = ft_strlen(cmd);
	uint32_t		i;
	char			*path;
	char			*cmd_with_path;

	path = find_env_var("PATH", 4, &i, m->envp);
	if (path == NULL || *path == '\0')
		return (cmd);
	while (*path != '\0')
	{
		i = 0;
		while (path[i] != ':' && path[i] != '\0')
			i++;
		cmd_with_path = arena_alloc(a, sizeof(char) * (i + cmd_str_len + 2));
		ft_memmove(cmd_with_path, path, i);
		cmd_with_path[i] = '/';
		ft_memmove(&cmd_with_path[i + 1], cmd, cmd_str_len);
		if (access(cmd_with_path, F_OK) == 0)
			return (cmd_with_path);
		arena_unalloc(a, sizeof(char) * (i + cmd_str_len + 2));
		path += i + (path[i] == ':');
	}
	command_not_found(m, cmd);
	return (NULL);
}

void run_command(t_minishell *m, char **argv)
{
	char *cmd_with_path;

	cmd_with_path = argv[0];
	if (ft_strchr(cmd_with_path, '/') == NULL)
		cmd_with_path = get_cmd_with_path(&m->node_arena, m, argv[0]); // replace with proper command finding function
	execve(cmd_with_path, argv, m->envp); // just have execve catch most error values
	execve_failure(m, argv[0]);
}

pid_t	execute_subprocess(t_minishell *m, char **argv, t_builtin builtin)
{
	pid_t	pid;

	// debug stuff
	for (int i = 0; argv[i] != NULL; ++i) // @TODO: remove this
	{
		//printf("i: %i\n", i);
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
			(void)execute_builtin(m, argv, builtin);
			error_exit(m, 0);
		}
		run_command(m, argv);
	}
	m->command_count += 1;
	return (pid);
}

int	execute_command(t_minishell *m, char **argv, int status)
{
	pid_t pid;
	t_builtin builtin_type;

	if (argv == NULL)
		return (1);
	if (status != 0)
	{
		pid = fork();
		if (pid == -1)
			syscall_failure(m); // @TODO: error cheking
		if (pid == 0)
			error_exit(m, 0);
		m->command_count += 1;
		m->last_pid = pid;
		return (1);
	}
	builtin_type = check_for_builtin(argv[0]);
	if (m->pipe_side == -1 && builtin_type != BUILTIN_FALSE)
	{
		execute_builtin(m, argv, builtin_type);
		return (1);
	}
	m->last_pid = execute_subprocess(m, argv, builtin_type);
	return (1);	
}
