/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:23:33 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/02 17:28:40 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <linux/limits.h>

#include "../includes/minishell.h" // fix maybe

void export(void)
{
	char *s = "vegenakki";
	printf("%s\n", s);

	//should return something?
}

void print_expansion(char *line)
{	
	//line++;
	printf("%s\n", getenv(line));	
}

char *get_token_name(t_token *token)
{
	if (token->type == PIPE)
		return("|");
	if (token->type == REDIRECT_IN)
		return("<");
	if (token->type == REDIRECT_OUT)
		return(">");
	if (token->type == REDIRECT_APPEND)
		return(">>");
	if (token->type == HERE_DOCUMENT)
		return("<<");
	if (token->type == WORD)
		return("word");
	if (token->type == END_OF_LINE)
		return ("newline");
	return ("ERROR");
}
//debug stuff
void print_token(t_token *token)
{
	// debug stuff
	printf("token number: %i\ttoken name: %s\ttoken string: %.*s\n",
			token->type, get_token_name(token), (int)token->string_len, token->u_data.string);
}

int	create_and_store_pipe(t_minishell *m, int8_t *side)
{
	if (*side == WRITE || *side == -1)
	{
		if (pipe(m->pipe) == -1)
			syscall_failure(m); //@TODO: error cheking
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


void close_heredocs(t_minishell *m)
{
	uint32_t i;

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

void	wait_for_sub_processes(t_minishell *minishell)
{
	uint32_t	i;
	int			wstatus;
	pid_t		pid;

	i = 0;
	printf("command count: %o\n", minishell->command_count); // Debug stuff
	printf("last_pid = %i\n", minishell->last_pid); // Debug stuff
	while (i < minishell->command_count)
	{
		pid = wait(&wstatus);
		if (pid == (pid_t)(-1))
			perror("wait issue");
		if (pid == minishell->last_pid)
		{
			printf("pid == %i\n", pid);
			printf("exit_status before = %i\n", minishell->exit_status);
			minishell->exit_status = WEXITSTATUS(wstatus);
			printf("exit_status after = %i\n", minishell->exit_status);

		}
		i++;
	}
}

int store_heredoc(t_minishell *m, int fd)
{
	uint32_t i;

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

int do_redir(t_minishell *m, t_token *data)
{
	int status;

	status = 0;
	if(data->type > 0) // maybe temp stuff
		status = store_heredoc(m, data->type); // delimiter will still have quotes removed
	else if (data->type == REDIRECT_OUT)
		status = redirect_out(data->u_data.argv, m);
	else if (data->type == REDIRECT_IN)
		status = redirect_in(data->u_data.argv, m);
	else if (data->type == REDIRECT_APPEND)
		status = redirect_append(data->u_data.argv, m);
	return (status);
}

int minishell_exec_loop(t_minishell *m, t_node *tree)
{
	t_node *current_head;
	int status;
	
	m->pipe_side = -1;
	status = 0;
	while (tree)
	{
		current_head = tree;
		if (m->pipe_side == READ)
			status = create_and_store_pipe(m, &m->pipe_side);
		if (tree->token.type == PIPE)
		{
			status = create_and_store_pipe(m, &m->pipe_side);
			tree = tree->left;
		}
		while (tree)
		{
			status = do_redir(m, &tree->token);
			if (tree->token.type == WORD || status != 0)
				execute_command(m, tree->token.u_data.argv, status);
			tree = tree->left;	
		}
		reset_redirect(m);
		tree = current_head->right;
	}
	return (0);
}

void read_loop(t_minishell *m)
{
	t_node *tree;
	uint32_t i;

	while (1)
	{
		i = 0;
		m->command_count = 0;
		m->heredoc_count = 0;
		m->line = readline("minishell> ");
		if (m->line == NULL)
			break ;
		m->line_counter += 1;
		while (is_space(m->line[i]))
			i++;
		if (m->line[i] == '\0')
			continue ;
		add_history(m->line); // bash would add a line with only spaces to the history. I dont think that makes any sense so we'll look at it later
		tree = parser(&m->node_arena, &m->line[i]);
		if (tree != NULL)
			minishell_exec_loop(m, tree);
		wait_for_sub_processes(m);
		free(m->line);
		m->line = NULL;
		arena_reset(&m->node_arena);
	}
}

void minishell_cleanup(t_minishell *minishell)
{
	arena_delete(&minishell->node_arena);
	close_heredocs(minishell);
	free(minishell->line);
}
// set default values for the minishell struct
// the struct is going to work as a kind of storage for globally needef alues
// we might want to pre allocate the arenas that will be used here to make cleanup easier
void init_minishell(t_minishell *minishell, char **envp)
{
	static int heredoc_fds_arr[16] = {0};

	minishell->node_arena = arena_new(DEFAULT_ARENA_CAPACITY);
	if (minishell->node_arena.data == NULL)
		; // @TODO: error cheking
	minishell->line = NULL;
	minishell->command_count = 0;
	minishell->line_counter = 0;
	minishell->exit_status = 0;
	minishell->heredoc_fds = heredoc_fds_arr;
	minishell->heredoc_count = 0;
	minishell->envp = envp; // need to be our own env etc.. bla bla bal
	minishell->redir_fds[READ] = STDIN_FILENO;
	minishell->redir_fds[WRITE] = STDOUT_FILENO;
	minishell->pipe[READ] = -1;
	minishell->pipe[WRITE] = -1;
	minishell->last_pid = 0; 
	minishell->pids = NULL;
	get_minishell(minishell); // this will probably not be used so remeber to take care of it
}

int main(int argc, char *argv[], char **envp)
{
	t_minishell minishell;

	(void)argc;
	(void)argv;
	init_minishell(&minishell, envp);
	read_loop(&minishell);
	minishell_cleanup(&minishell);
	printf("exit\n"); // doesnt make sense
    return (minishell.exit_status);
}
