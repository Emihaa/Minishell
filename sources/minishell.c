/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:23:33 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/26 21:48:19 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <linux/limits.h>

#include "../includes/minishell.h" // fix maybe

void env(char **envp)
{
	int i;

	i = 0;
	if (envp == NULL)
		return ;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
}

void echo(int argc, char *argv[])
{
	int i;

	i = 0;
	while (i < argc)
	{
		printf("%s", argv[i]);
		i++;
	}
	if (ft_strncmp(argv[0], "-n", 3) == 0)
		return ;
	printf("\n");
}

//pwd command
//propably need file dupping (dup2)
void print_working_directory(void)
{
	char current_path[PATH_MAX];
	getcwd(current_path, PATH_MAX);
	printf("%s\n", current_path);
}

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

// void read_loop(char **envp)
// {
// 	char *line;
// 	while (1)
// 	{
// 		line = readline("minishell> ");
// 		add_history(line);
//     	//printf("%s", line);
// 		if (strncmp(line, "env", 4) == 0)
// 		{
// 			env(envp);
// 			continue;
// 		}
// 		if (strncmp(line, "pwd", 4) == 0)
// 		{
// 			print_working_directory();
// 			continue;
// 		}
// 		if (strncmp(line, "export", 4) == 0)
// 		{
// 			print_export();
// 			continue;
// 		}
// 		if (*line == '$')
// 		{
// 			print_expansion(line);
// 			continue;
// 		}
// 		if (strncmp(line, "exit", 5) == 0)
// 			return ;
// 	}
// }

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

void print_token(t_token *token)
{
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
// in the future this will actually need the argv inside the tree node
// rename probably

static inline
void close_pipe(t_minishell *m)
{
	if (m->pipe[WRITE] != -1)
		if (close(m->pipe[WRITE]))
			syscall_failure(m);
	if (m->pipe[READ] != -1)
		if (close(m->pipe[READ]))
			syscall_failure(m);
}

void builtin_exit(char **argv, t_minishell *m)
{
	(void)argv;
	minishell_cleanup(m);
	if (m->pipe_side != -1)
		write(2, "exit\n", 5);
	exit(m->exit_status);
}

t_builtin check_for_builtin(char *command)
{
	if (ft_strncmp(command, "exit", 5) == 0)
		return(BUILTIN_EXIT);
	if (ft_strncmp(command, "echo", 5) == 0)
		return(BUILTIN_ECHO);
	if (ft_strncmp(command, "cd", 3) == 0)
		return(BUILTIN_CD);
	if (ft_strncmp(command, "pwd", 4) == 0)
		return(BUILTIN_PWD);
	if (ft_strncmp(command, "env", 4) == 0)
		return(BUILTIN_ENV);
	if (ft_strncmp(command, "unset", 5) == 0)
		return(BUILTIN_UNSET);
	if (ft_strncmp(command, "export", 5) == 0)
		return(BUILTIN_EXPORT);
	return (BUILTIN_FALSE);
}

int	execute_builtin(t_minishell *m, char **argv, t_builtin command)
{
	if (command == BUILTIN_EXIT)
		builtin_exit(argv, m);
	if (command == BUILTIN_ECHO)
		; // @TODO: add command
	if (command == BUILTIN_CD)
		; // @TODO: add command
	if (command == BUILTIN_PWD)
		; // @TODO: add command
	if (command == BUILTIN_ENV)
		; // @TODO: add command
	if (command == BUILTIN_UNSET)
		; // @TODO: add command
	if (command == BUILTIN_EXPORT)
		; // @TODO: add command
	return (0);
}

pid_t	execute_subprocess(t_minishell *m, char **argv, t_builtin builtin)
{
	pid_t	pid;
	int		status = 1;
	for (int i = 0; argv[i] != NULL; ++i)
	{
		printf("i: %i\n", i);
		printf("argv[%i]: %s\n", i, argv[i]);
	}
	pid = fork();
	if (pid == (pid_t)(-1))
		; // @TODO: error cheking
	if (pid == 0)
	{
		if (builtin != BUILTIN_FALSE)
			execute_builtin(m, argv, builtin);
		apply_redirect(m);
		close_pipe(m);
		char *path = ft_strjoin("/usr/bin/", argv[0]); // replace with proper command finding function
		if (execve(path, argv, m->envp) == -1) // just have execve catch most error values
			perror("execve fail");
		printf("errno: %i\n", errno);
		if (errno == ENOENT)
			status = 127;
		perror(argv[0]);
		error_exit(m, status);
	}
	m->command_count += 1;
	return (pid);
}

void execute_command(t_minishell *m, char **argv, int8_t pipe_side, int status)
{
	pid_t pid;
	t_builtin builtin;

	if (argv == NULL)
		return ;
	if (status != 0)
	{
		pid = fork();
		if (pid == -1)
			; // @TODO: error cheking
		if (pid == 0)
			error_exit(m, status);
		m->command_count += 1;
		m->last_pid = pid;
		return ;
	}
	builtin = check_for_builtin(argv[0]);
	if (pipe_side == -1)
	{
		if (execute_builtin(m, argv, builtin))
			return ;
	}
	m->last_pid = execute_subprocess(m, argv, builtin);
}

void	wait_for_sub_processes(t_minishell *minishell)
{
	uint32_t	i;
	int			wstatus;
	pid_t		pid;

	i = 0;
	printf("command count: %o\n", minishell->command_count);
	printf("last_pid = %i\n", minishell->last_pid);
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

int minishell_exec_loop(t_minishell *m, t_node *tree)
{
	t_node *current_head;
	int8_t pipe_side;
	int status;
	
	pipe_side = -1;
	status = 0;
	while (tree)
	{
		current_head = tree;
		if (pipe_side == READ)
			status = create_and_store_pipe(m, &pipe_side);
		if (tree->token.type == PIPE)
			status = create_and_store_pipe(m, &pipe_side);
		while (tree)
		{
			if(tree->token.type == HERE_DOCUMENT) // maybe temp stuff
				status = heredoc(m, &tree->token); // delimiter will still have quotes removed
			else if (tree->token.type == REDIRECT_OUT)
				status = redirect_out(tree->token.u_data.argv, m);
			else if (tree->token.type == REDIRECT_IN)
				status = redirect_in(tree->token.u_data.argv, m);
			else if (tree->token.type == REDIRECT_APPEND)
				status = redirect_append(tree->token.u_data.argv, m);
			else if (tree->token.type == WORD)
			{
				execute_command(m, tree->token.u_data.argv, pipe_side, status);
				break ;
			}
			tree = tree->left;	
		}
		reset_redirect(m);
		tree = current_head->right;
	}
	printf("arena_size: %lu", m->node_arena.size);
	return (42);
}

void read_loop(t_minishell *m)
{
	t_node *tree;

	while (1)
	{
		m->command_count = 0;
		m->line = readline("minishell> ");
		if (m->line == NULL)
			break ;
		if (*m->line == '\0')
			continue ;
		add_history(m->line);
		m->line_counter += 1;
		tree = parser(&m->node_arena, m->line);
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
	arena_delete(&minishell->scratch_arena);
	arena_delete(&minishell->env_arena);
	free(minishell->line);
}
// set default values for the minishell struct
// the struct is going to work as a kind of storage for globally needef alues
// we might want to pre allocate the arenas that will be used here to make cleanup easier
void init_minishell(t_minishell *minishell, char **envp)
{
	minishell->node_arena = arena_new(DEFAULT_ARENA_CAPACITY);
	if (minishell->node_arena.data == NULL)
		; //@TODO: error cheking
	minishell->env_arena = arena_new(ARG_MAX);
	if (minishell->env_arena.data == NULL)
		; //@TODO: error cheking
	minishell->scratch_arena = arena_new(1024);
	if (minishell->scratch_arena.data == NULL)
		; //@TODO: error cheking
	minishell->line = NULL;
	minishell->command_count = 0;
	minishell->line_counter = 0;
	minishell->exit_status = 0;
	minishell->heredoc_count = 0;
	minishell->envp = envp; // need to be our own env etc.. bla bla bal
	minishell->redir_fds[READ] = STDIN_FILENO;
	minishell->redir_fds[WRITE] = STDOUT_FILENO;
	minishell->pipe[READ] = -1;
	minishell->pipe[WRITE] = -1;
	minishell->last_pid = 0; 
	minishell->pids = NULL;
	get_minishell(minishell);
}

int main(int argc, char *argv[], char *envp[])
{
	t_minishell minishell;
	(void)argc;
	(void)argv;
	init_minishell(&minishell, envp);
	read_loop(&minishell);
	minishell_cleanup(&minishell);
	printf("exit\n");
    return (0);
}
