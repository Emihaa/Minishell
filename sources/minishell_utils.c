/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 03:44:08 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/25 04:21:01 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtin.h"

void	wait_signal_handler(int signal)
{
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
	}
}

int	read_loop_event_hook(void)
{
	if (g_int == SIGINT)
	{
		get_minishell(NULL)->exit_status = 128 + SIGINT;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_int = 0;
		return (1);
	}
	return (0);
}

void	signal_handler(int signal)
{
	g_int = signal;
}

void	minishell_cleanup(t_minishell *minishell)
{
	arena_delete(minishell->global_arena);
	arena_delete(minishell->file_buf);
	close_heredocs(minishell);
	free(minishell->line);
	while (minishell->envp_size-- > 0)
		free(minishell->envp[minishell->envp_size]);
	free(minishell->envp);
}

// set default values for the minishell struct
// the struct is going to work as a kind of storage for globally needef alues
// we might want to pre allocate the arenas that 
// will be used here to make cleanup easier
void	init_minishell(t_minishell *minishell, char **envp)
{
	static int	heredoc_fds_arr[16] = {0};

	minishell->istty = isatty(STDIN_FILENO);
	minishell->global_arena = xarena_new(DEFAULT_ARENA_CAPACITY);
	minishell->file_buf = NULL;
	minishell->line = NULL;
	minishell->command_count = 0;
	minishell->line_counter = 0;
	minishell->exit_status = 0;
	minishell->heredoc_fds = heredoc_fds_arr;
	minishell->heredoc_count = 0;
	minishell->envp_size = 0;
	minishell->env_capacity = 64;
	minishell->envp = create_env(minishell, envp);
	if (!minishell->envp)
	{
		put_str(STDERR_FILENO, "allocation failure\n");
		error_exit(minishell, 1); // @TODO: error checking
	}
	minishell->redir_fds[READ] = STDIN_FILENO;
	minishell->redir_fds[WRITE] = STDOUT_FILENO;
	minishell->pipe[READ] = -1;
	minishell->pipe[WRITE] = -1;
	minishell->last_pid = 0;
	get_minishell(minishell);
}
