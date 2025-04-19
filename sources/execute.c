/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 21:05:55 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/20 01:25:50 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static inline
void	delete_temp_string(t_string *str)
{
	arena_unalloc(str->memory, str->size);
	str->memory = NULL;
	str->base = NULL;
	str->size = 0;
	str->capacity = 0;
}

char *get_cmd_with_path(t_arena *a, t_minishell *m, char *path, char *cmd)
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
		if (string_find_new_memory(a, &str, i + cmd_str_len + 2))
		 	syscall_failure(m); // error exit?
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

// char *get_cmd_with_path(t_arena *a, t_minishell *m, char *path, char *cmd)
// {
// 	const uint32_t	cmd_str_len = ft_strlen(cmd);
// 	uint32_t		i;
// 	t_string		str;

// 	if (path == NULL || *path == '\0')
// 		return (cmd);
// 	str = (t_string){0};
// 	while (*path != '\0')
// 	{
// 		i = 0;
// 		while (path[i] != ':' && path[i] != '\0')
// 			i++;
// 		char *full_path = xarena_alloc(a, i + cmd_str_len + 2);
// 		ft_memmove(full_path, path, i);
// 		full_path[i] = '/';
// 		ft_memmove(&full_path[i + 1], cmd, cmd_str_len);
// 		if (access(full_path, F_OK) == 0)
// 			return (str.base);
// 		arena_unalloc(a, i + cmd_str_len + 2);
// 		path += i + (path[i] == ':'); // this is a problem because the arena that path was allocated on might not be the base // I will rethink the arena implementation again later
// 	}
// 	command_not_found(m, cmd);
// 	return (NULL);
// }


void run_command(t_arena *arena, t_minishell *m, char **argv)
{
	char *path;
	char *cmd_with_path;

	cmd_with_path = argv[0];
	if (ft_strchr(cmd_with_path, '/') == NULL)
	{
		path = get_env_var_value("PATH", 4);
		cmd_with_path = get_cmd_with_path(arena, m, path, argv[0]); // replace with proper command finding function
	}
	// debug stuff start
	// t_arena *temp = arena;
	// size_t total = 0;
	// for (int i = 0; temp != NULL; i++)
	// {
	// 	total += temp->capacity;
	// 	printf("arena region[%i] size: <%lu> capacity <%lu>\n", i, temp->size, temp->capacity);
	// 	printf("data of [%i]as chars: %.*s\n", i, (int)temp->capacity, temp->data);
	// 	temp = temp->next;
	// }
	// printf("total: <%lu>\n", total);
	// debug stuff end
	execve(cmd_with_path, argv, m->envp); // just have execve catch most error values
	execve_failure(m, argv[0]);
}

pid_t	execute_subprocess(t_arena *arena, t_minishell *m, char **argv, t_builtin builtin)
{
	pid_t	pid;

	// debug stuff start
	// for (int i = 0; argv[i] != NULL; ++i) // @TODO: remove this
	// {
	// 	//printf("i: %i\n", i);
	// 	printf("argv[%i]: %s\n", i, argv[i]);
	// }
	// debug stuff end
	pid = fork();
	if (pid == (pid_t)(-1))
		syscall_failure(m); // @TODO: error cheking
	if (pid == 0)
	{ // we are inside child, so ignore SIGINT in parent
		apply_redirect(m);
		close_pipe(m);
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
	pid_t pid;
	t_builtin builtin_type;

	if (argv == NULL || argv[0] == NULL)
		return (1);
	if (status != 0)
	{
		pid = fork();
		if (pid == -1)
			syscall_failure(m); // @TODO: error cheking
		if (pid == 0)
			error_exit(m, 1); // @todo check if this exit code is correct
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
	m->last_pid = execute_subprocess(arena, m, argv, builtin_type);
	return (1);
}
