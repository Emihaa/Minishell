/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 21:05:55 by ltaalas           #+#    #+#             */
/*   Updated: 2025/05/06 17:39:03 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/builtin.h"

static inline
void	delete_temp_string(t_string *str)
{
	arena_unalloc(str->memory, str->size);
	str->memory = NULL;
	str->base = NULL;
	str->size = 0;
	str->capacity = 0;
}

char	*get_cmd_with_path(t_arena *a, t_minishell *m, char *path, char *cmd)
{
	const uint32_t	cmd_str_len = ft_strlen(cmd);
	uint32_t		i;
	t_string		str;

	if (path == NULL || *path == '\0')
		return (cmd);
	str = (t_string){0};
	while (*path != '\0')
	{
		i = 0;
		while (path[i] != ':' && path[i] != '\0')
			i++;
		string_find_new_memory(a, &str, i + cmd_str_len + 2);
		append_to_string(a, &str, path, i);
		append_to_string(a, &str, "/", 1);
		append_to_string(a, &str, cmd, cmd_str_len);
		terminate_and_commit_string(a, &str);
		if (access(str.base, F_OK) == 0)
			return (str.base);
		delete_temp_string(&str);
		path += i + (path[i] == ':');
	}
	command_not_found(m, cmd);
	return (NULL);
}

void	run_command(t_arena *arena, t_minishell *m, char **argv)
{
	char	*path;
	char	*cmd_with_path;

	cmd_with_path = argv[0];
	if (ft_strchr(cmd_with_path, '/') == NULL)
	{
		path = get_env_var_value("PATH", 4);
		cmd_with_path = get_cmd_with_path(arena, m, path, argv[0]);
	}
	execve(cmd_with_path, argv, m->envp);
	execve_failure(m, argv[0]);
}

pid_t	execute_subprocess(t_arena *arena, t_minishell *m,
		char **argv, t_builtin builtin)
{
	pid_t	pid;

	pid = fork();
	if (pid == (pid_t)(-1))
		syscall_failure(m, __FILE__, __LINE__);
	if (pid == 0)
	{
		apply_redirect(m);
		close_pipe(m);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (builtin != BUILTIN_FALSE)
		{
			(void)execute_builtin(m, argv, builtin);
			error_exit(m, 0);
		}
		run_command(arena, m, argv);
	}
	m->command_count += 1;
	return (pid);
}

int	execute_command(t_arena *arena, t_minishell *m, char **argv, int status)
{
	pid_t		pid;
	t_builtin	builtin_type;

	if (argv == NULL || argv[0] == NULL)
		return (1);
	if (status != 0)
	{
		pid = fork();
		if (pid == -1)
			syscall_failure(m, __FILE__, __LINE__);
		if (pid == 0)
			error_exit(m, 1);
		m->command_count += 1;
		m->last_pid = pid;
		return (1);
	}
	builtin_type = check_for_builtin(argv[0]);
	if (m->pipe_side == -1 && builtin_type != BUILTIN_FALSE)
	{
		execute_builtin_in_main_process(m, argv, builtin_type);
		return (1);
	}
	m->last_pid = execute_subprocess(arena, m, argv, builtin_type);
	return (1);
}
