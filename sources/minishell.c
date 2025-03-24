/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:23:33 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/25 01:06:53 by ltaalas          ###   ########.fr       */
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

// prototypee
// void minishell_exec_loop(t_minishell *minishell, t_arena *arena, t_token *token_array)
// {
// 	char **envp = minishell->envp;
// 	int i;
// 	i = 0;
// 	while (token_array[i].type != END_OF_LINE)
// 	{
// 		if (token_array[i].type == WORD)
// 		{
// 			if (ft_strncmp("exit", token_array[i].string, token_array[i].string_len) == 0)
// 				break;
// 		}
// 		else if(token_array[i].type == HERE_DOCUMENT) // fully temp stuff
// 		{
// 			char *delimiter = calloc(1, token_array[i].string_len + 1);
// 			ft_memmove(delimiter, token_array[i].string, token_array[i].string_len);
// 			heredoc(minishell, delimiter); // delimiter will still have quotes removed
// 			free(delimiter);
// 		}
// 		else
// 		{
// 			print_token(&token_array[i]);
// 		}
// 		++i;
// 	}
// 	pid_t pid = fork();
// 	if (pid == 0)
// 	{
// 		apply_redirect(minishell);
// 		char *cat_argv[3] = {[0] = "cat", "-e", [2] = NULL};
// 		printf("fd to cat \t%i\n", 17);
// 		if (execve("/bin/cat", cat_argv, envp) == -1)
// 			perror("execve fail");
// 		exit(1);
// 	}
// 	reset_redirect(minishell);
// 	minishell->pids = arena_alloc(arena, sizeof(minishell->pids));
// 	minishell->pids[minishell->command_count] = pid;
// 	minishell->command_count += 1;
// 	wait_for_sub_processes(minishell);
// 	printf("last printf %s\t%i\n", strerror(minishell->exit_status), minishell->exit_status);
// }
// void minishell_exec_loop(t_minishell *minishell, t_arena *arena, t_node *tree)
// {
// 	char **envp = minishell->envp;
// 	int i;
// 	i = 0;
// 	while (tree->left)
// 	{
// 		if (tree->token.type == PIPE)
// 			// do pipe and do fork to child
// 			// and child work
// 			// call itself but tree->right
// 			// that would be with recursion
// 		if (token_array[i].type == WORD)
// 		{
// 			if (ft_strncmp("exit", token_array[i].string, token_array[i].string_len) == 0)
// 				break;
// 		}
// 		else if(token_array[i].type == HERE_DOCUMENT) // fully temp stuff
// 		{
// 			char *delimiter = calloc(1, token_array[i].string_len + 1);
// 			ft_memmove(delimiter, token_array[i].string, token_array[i].string_len);
// 			heredoc(minishell, delimiter); // delimiter will still have quotes removed
// 			free(delimiter);
// 		}
// 		else
// 		{
// 			print_token(&token_array[i]);
// 		}
// 		++i;
// 	}
// 	pid_t pid = fork();
// 	if (pid == 0)
// 	{
// 		apply_redirect(minishell);
// 		char *cat_argv[3] = {[0] = "cat", "-e", [2] = NULL};
// 		printf("fd to cat \t%i\n", 17);
// 		if (execve("/bin/cat", cat_argv, envp) == -1)
// 			perror("execve fail");
// 		exit(1);
// 	}
// 	reset_redirect(minishell);
// 	minishell->pids = arena_alloc(arena, sizeof(minishell->pids));
// 	minishell->pids[minishell->command_count] = pid;
// 	minishell->command_count += 1;
// 	wait_for_sub_processes(minishell);
// 	printf("last printf %s\t%i\n", strerror(minishell->exit_status), minishell->exit_status);
// }

int	create_and_store_pipe(t_minishell *m, bool *side)
{
	int return_val;

	if (*side == WRITE)
	{
		if (pipe(m->pipe) == -1)
			return (ANTIKRISTA); //@TODO: error cheking
		return_val = store_write_fd(m->pipe[WRITE], m);
		m->pipe[WRITE] = -1;
		*side = READ;
		return (return_val);
	}
	else if (*side == READ)
	{
		return_val = store_read_fd(m->pipe[READ], m);
		m->pipe[READ] = -1;
		*side = WRITE;
		return (return_val);
	}
	return (0);
}

void exit_minishell(t_minishell *m, int exit_status)
{
	minishell_cleanup(m);
	exit(exit_status);
}
// in the future this will actually need the argv inside the tree node
pid_t	handle_word(t_minishell *m, char **argv, int status) // rename probably
{
	pid_t pid;

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
		if (status != 0)
			exit_minishell(m, status);
		// printf("fds to apply \t\t%i\t%i\n", m->redir_fds[READ], m->redir_fds[WRITE]);
		apply_redirect(m);
		if (m->pipe[WRITE] != -1)
			if (close(m->pipe[WRITE]))
				perror("pipe[WRITE] close");
		if (m->pipe[READ] != -1)
			if (close(m->pipe[READ]))
				perror("pipe[READ] close");
		// printf("fds after application \t%i\t%i\n", m->redir_fds[READ], m->redir_fds[WRITE]);
		//char *cat_argv[3] = {[0] = "cat", [1] = NULL, [2] = NULL}; //@TODO <- do this to tree and expand it
		char *path = ft_strjoin("/usr/bin/", argv[0]);
		if (execve(path, argv, m->envp) == -1)
			perror("execve fail");
		printf("errno: %i\n", errno);
		if (errno == ENOENT)
			status = 127;
		exit_minishell(m, status);
	}
	m->command_count += 1;
	return (pid);
}

static
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
			printf("exit_status = %i\n", minishell->exit_status);
			minishell->exit_status = WEXITSTATUS(wstatus);
		}
		i++;
	}
}

int minishell_exec_loop(t_minishell *m, t_arena *arena, t_node *tree)
{
	(void)arena;
	bool pipe_side;
	int status;
	t_node *current_head;
	
	pipe_side = WRITE;
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
			{
				heredoc(m, &tree->token); // delimiter will still have quotes removed
			}
			// else if (tree->token.type == REDIRECT_OUT)
			// {
				
			// }
			else if (tree->token.type == WORD)
			{
				if (ft_strncmp("exit", tree->token.u_data.string, tree->token.string_len) == 0) // doesn't work the same as bash
					return (EXIT_SUCCESS);
				m->last_pid = handle_word(m, tree->token.u_data.argv, status);
				break ;
			}
			else
			{
				print_token(&tree->token); // for testing
			}
			tree = tree->left;	
		}
		reset_redirect(m);
		tree = current_head->right;
	}
	wait_for_sub_processes(m);
	// printf("last print of loop %s\t%i\n", strerror(m->exit_status), m->exit_status);
	return (42);
}

// prototype for a read loop
// this should probably call the parser which will call the lexer and return the tree
// that will then be passed to some execution prep function which will traverses the tree 
// 		and do the necessary redirect ect.
//this might need another nested loop to calculate the amount of command etc.
// void read_loop(t_minishell *minishell)
// {
// 	char *line;
// 	t_arena *arena = &minishell->node_arena;
// 	t_token *token_array; // just for testing
// 	t_lexer lexer; // to be definex in the parser
// 	while (1)
// 	{
// 		minishell->command_count = 0;
// 		line = readline("minishell> ");
// 		if (line == NULL)
// 			break ; // @TODO: error cheking
// 		add_history(line);
//     	//printf("%s", line);
// 		minishell->line_counter += 1;
// 		lexer.line = line;
// 		lexer.line_index = 0;
// 		token_array = get_token_array(arena, &lexer); //<--
// 		minishell_exec_loop(minishell, arena, token_array);
// 		arena_reset(arena);
// 	}
// }

void read_loop(t_minishell *m)
{
	char *line;

	while (1)
	{
		m->command_count = 0;
		line = readline("minishell> ");
		if (line == NULL)
		{
			printf("readline returned NULL: errno = %i\n", errno);
			perror("readline in read_loop");
			break ; // @TODO: error cheking
		}
		if (*line == '\0')
			continue ;
		add_history(line);
    	//printf("%s", line);
		m->line_counter += 1;
		t_node *tree = parser(&m->node_arena, line);
		if (tree != NULL)
			if (minishell_exec_loop(m, &m->node_arena, tree) == EXIT_SUCCESS)
				break ;
		free(line);
		arena_reset(&m->node_arena);
	}
}

void minishell_cleanup(t_minishell *minishell)
{
	arena_delete(&minishell->node_arena);
	arena_delete(&minishell->scratch_arena);
	arena_delete(&minishell->env_arena);
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
	minishell->envp = envp;
	minishell->command_count = 0;
	minishell->line_counter = 0;
	minishell->exit_status = 0;
	minishell->heredoc_count = 0;
	minishell->envp = envp; // need to be our own env etc.. bla bla bal
	minishell->redir_fds[READ] = STDIN_FILENO;
	minishell->redir_fds[WRITE] = STDOUT_FILENO;
	minishell->pipe[READ] = -1;
	minishell->pipe[WRITE] = -1;
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


// heredoc testing main
// currently using as reference
// int main(int argc, char *argv[], char *envp[])
// {
// 	pid_t pids[3] = {0};
// 	t_minishell minishell;
// 	init_minishell(&minishell);
// 	(void)argc;
// 	(void)argv;
// 	int wstatus;
// 	//read_loop(envp);
// 	printf("%i\n", getpid());
// 	while (minishell.command_count < 2)
// 	{
// 		heredoc(&minishell, "DELIM");
// 		pid_t pid = fork();
// 		if (pid == 0)
// 		{
// 			apply_redirect(&minishell);
// 			char *cat_argv[3] = {[0] = "cat", "-e", [2] = NULL};
// 			printf("fd to cat \t%i\n", 17);
// 			if (execve("/bin/cat", cat_argv, envp) == -1)
// 				perror("execve fail");
// 			exit(1);
// 		}
// 		reset_redirect(&minishell);
// 		pids[minishell.command_count] = pid;
// 		minishell.command_count += 1;
// 	}
// 	int exit_status = wait_for_sub_processes(&minishell, pids);
// 	printf("last printf %s\t%i\n", strerror(exit_status), exit_status);
// 	printf("exit\n");
//     return (0);
// }