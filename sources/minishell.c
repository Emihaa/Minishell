/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:23:33 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/25 00:03:16 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <linux/limits.h>

#include "../includes/minishell.h" // fix maybe

volatile sig_atomic_t g_int = 0;

/// @todo error cheking
int	create_and_store_pipe(t_minishell *m, int8_t *side)
{
	if (*side == WRITE || *side == -1)
	{
		if (pipe(m->pipe) == -1)
			syscall_failure(m);
		store_write_fd(m->pipe[WRITE], m);
		m->pipe[WRITE] = -1;
		*side = READ;
		return (0);
	}
	else if (*side == READ)
	{
		store_read_fd(m->pipe[READ], m);
		m->pipe[READ] = -1;
		*side = WRITE;
		return (0);
	}
	return (0);
}

void	close_heredocs(t_minishell *m)
{
	uint32_t	i;

	i = 0;
	while(i < m->heredoc_count)
	{
		if (m->heredoc_fds[i] != -1)
		{
			if (close(m->heredoc_fds[i]) == -1)
				syscall_failure(m);
		}
		i++;
	}
}

void	wait_signal_handler(int signal)
{
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
	}
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
		i++;
	}
	signal(SIGINT, signal_handler);
}

int	store_heredoc(t_minishell *m, int fd)
{
	uint32_t	i;

	i = 0;
	store_read_fd(fd, m);
	while (i < m->heredoc_count)
	{
		if (m->heredoc_fds[i] == fd)
		{
			m->heredoc_fds[i] = -1;
			break ;
		}
		i++;
	}
	return (0);
}

int	do_redir(t_minishell *m, t_token *data)
{
	int	status;

	status = 0;
	if(data->type > 0) // maybe temp stuff
		status = store_heredoc(m, data->type); // delimiter will still have quotes removed
	else if (data->type == REDIRECT_OUT)
		status = redirect_out(data->string, m);
	else if (data->type == REDIRECT_IN)
		status = redirect_in(data->string, m);
	else if (data->type == REDIRECT_APPEND)
		status = redirect_append(data->string, m);
	else if (data->type == REDIRECT_AMBI)
		status = redirect_ambi(data->string);
	return (status);
}

int	minishell_exec_loop(t_arena *arena, t_minishell *m, t_node *tree)
{
	t_node	*current_head;
	int		status;

	m->pipe_side = -1;
	status = 0;
	while (tree)
	{
		current_head = tree;
		if (m->pipe_side == READ)
			status = create_and_store_pipe(m, &m->pipe_side);
		if (tree->token.type == PIPE)
			status = create_and_store_pipe(m, &m->pipe_side);
		while (tree)
		{
			status = do_redir(m, &tree->token);
			if (tree->token.type == WORD || status != 0)
				if (execute_command(arena, m, tree->token.argv, status))
					break ;
			tree = tree->left;
		}
		reset_redirect(m);
		tree = current_head->right;
	}
	return (0);
}

// @TODO cntrl+C on CAT gives two new newlines
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
		add_history(m->line); // bash would add a line with only spaces to the history. I dont think that makes any sense so we'll look at it later
		tree = parser(m->global_arena, m, &m->line[i]);
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

void	minishell_cleanup(t_minishell *minishell)
{
	arena_delete(minishell->global_arena);
	arena_delete(minishell->file_buf);
	close_heredocs(minishell);
	free(minishell->line);
	while (minishell->envp_size >= 0)
		free(minishell->envp[minishell->envp_size--]);
	free(minishell->envp);
}

// set default values for the minishell struct
// the struct is going to work as a kind of storage for globally needef alues
// we might want to pre allocate the arenas that will be used here to make cleanup easier
void	init_minishell(t_minishell *minishell, char **envp)
{
	static int heredoc_fds_arr[16] = {0};

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
	minishell->envp = create_env(minishell, envp); // <---------- @TODO Emilia
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

// maybe this should just be setting the g_int = signal
void	signal_handler(int signal)
{
	g_int = signal;
}

// SIGINT for cntrl+C
// cancel current input, prints a newline + promt
// in child rocesses reset signal to default so they get killed properly
// signal status, ON_READ <- default status of the shell
// ON_HDOC <-- shell using stdin and writing on hdoc
// ON_EXE, which shell is on while commands are being executed


// SIGQUIT for ctrl+\ <- this does nothing
// ignore the SIGQUIT, and reset it to default?

// ctrl+D EOF (End Of File) <- doesnt send signal but must be handled manually
// returns NULL to readline(), clean up and exit
// but only does this if input is empty
// this one already works

// @TODO: if we are on child process then we dont want the parent to get any signals
// so we need to have the signal(SIGINT, SIG_ING); on parent if we are on child process
// remember also to return the signal handling
int	main(int argc, char *argv[], char **envp)
{
	t_minishell	minishell;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);

	(void)argc;
	(void)argv;
	init_minishell(&minishell, envp);
	if (minishell.istty)
		read_loop(&minishell);
	else
		exec_mode(&minishell);
	minishell_cleanup(&minishell);
	put_str(STDERR_FILENO, "exit\n");
    return (minishell.exit_status);
}
