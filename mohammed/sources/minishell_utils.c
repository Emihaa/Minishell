/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 03:44:08 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/05/06 19:32:04 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtin.h"
#include "../includes/global.h"

void	wait_signal_handler(int signal)
{
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
	}
}

void	signal_handler(int signal)
{
	g_sig = signal;
}

void	minishell_cleanup(t_minishell *minishell)
{
	arena_delete(minishell->global_arena);
	arena_delete(minishell->file_buf);
	close_heredocs(minishell);
	free(minishell->line);
	if (minishell->envp)
	{
		while (minishell->envp_size-- > 0)
			free(minishell->envp[minishell->envp_size]);
		free(minishell->envp);
	}
	close_pipe(minishell);
	if (minishell->stdout_dup != -1)
	{
		close(minishell->stdout_dup);
		minishell->stdout_dup = -1;
	}
	reset_redirect(minishell);
}

static
void	envp_alloc_error(t_minishell *m)
{
	put_str(STDERR_FILENO, "minishell: allocation failure\n");
	error_exit(m, 1);
}

/// @brief initializes default values for the minishell struct
/// for globally used values
void	init_minishell(t_minishell *minishell, char **envp)
{
	static int	heredoc_fds_arr[16];

	get_minishell(minishell);
	ft_memset(heredoc_fds_arr, -1, sizeof(heredoc_fds_arr));
	ft_memset(minishell, 0, sizeof(*minishell));
	minishell->istty = isatty(STDIN_FILENO);
	minishell->file_buf = NULL;
	minishell->line = NULL;
	minishell->command_count = 0;
	minishell->line_counter = 0;
	minishell->exit_status = 0;
	minishell->heredoc_fds = heredoc_fds_arr;
	minishell->heredoc_count = 0;
	minishell->envp_size = 0;
	minishell->env_capacity = 64;
	minishell->redir_fds[READ] = STDIN_FILENO;
	minishell->redir_fds[WRITE] = STDOUT_FILENO;
	minishell->pipe[READ] = -1;
	minishell->pipe[WRITE] = -1;
	minishell->stdout_dup = -1;
	minishell->last_pid = 0;
	minishell->global_arena = xarena_new(DEFAULT_ARENA_CAPACITY);
	minishell->envp = create_env(minishell, envp);
	if (!minishell->envp)
		envp_alloc_error(minishell);
}
