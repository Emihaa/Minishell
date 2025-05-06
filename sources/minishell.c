/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:23:33 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/30 19:34:30 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/builtin.h"
#include "../includes/parser.h"
#include "../includes/heredoc.h"

volatile sig_atomic_t	g_int = 0;

static
void	set_exit_status(t_minishell *minishell, int wstatus)
{
	if (WIFSIGNALED(wstatus))
	{
		minishell->exit_status = 128 + WTERMSIG(wstatus);
		if (__WCOREDUMP(wstatus))
			put_str(STDERR_FILENO, "Quit (core dumped)\n");
	}
	else if (WIFEXITED(wstatus))
		minishell->exit_status = WEXITSTATUS(wstatus);
	else
		minishell->exit_status = 1;
}

void	wait_for_sub_processes(t_minishell *minishell)
{
	uint32_t	i;
	int			wstatus;
	pid_t		pid;

	i = 0;
	signal(SIGINT, &wait_signal_handler);
	while (i < minishell->command_count)
	{
		pid = wait(&wstatus);
		if (pid == (pid_t)(-1))
			perror("wait issue");
		if (pid == minishell->last_pid)
		{
			set_exit_status(minishell, wstatus);
		}
		i++;
	}
	signal(SIGINT, signal_handler);
}

void	read_loop(t_minishell *m)
{
	t_node		*tree;
	uint32_t	i;

	while (1)
	{
		free(m->line);
		m->line = NULL;
		rl_event_hook = read_loop_event_hook;
		m->command_count = 0;
		m->heredoc_count = 0;
		m->line = readline("minishell> ");
		if (m->line == NULL)
			break ;
		m->line_counter += 1;
		i = eat_space(m->line);
		if (m->line[i] == '\0')
			continue ;
		tree = parser(m->global_arena, m, &m->line[i]);
		add_history(m->line);
		if (tree != NULL)
			minishell_exec_loop(m->global_arena, m, tree);
		wait_for_sub_processes(m);
		arena_trim(m->global_arena);
		arena_reset(m->global_arena);
	}
}

void	exec_mode(t_minishell *m)
{
	t_node		*tree;
	uint32_t	i;

	m->file_buf = xarena_new(4096 + 1);
	while (1)
	{
		i = 0;
		m->command_count = 0;
		m->heredoc_count = 0;
		m->line = get_line(m->file_buf, STDIN_FILENO);
		if (m->line == NULL)
			error_exit(m, m->exit_status);
		m->line_counter += 1;
		tree = parser(m->global_arena, m, &m->line[i]);
		if (tree != NULL)
			minishell_exec_loop(m->global_arena, m, tree);
		wait_for_sub_processes(m);
		free(m->line);
		m->line = NULL;
		arena_trim(m->global_arena);
		arena_reset(m->global_arena);
	}
}

int	main(int argc, char *argv[], char **envp)
{
	t_minishell	minishell;

	(void)argc;
	(void)argv;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	init_minishell(&minishell, envp);
	if (minishell.istty)
		read_loop(&minishell);
	else
		exec_mode(&minishell);
	minishell_cleanup(&minishell);
	put_str(STDERR_FILENO, "exit\n");
	return (minishell.exit_status);
}
