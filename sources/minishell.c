/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:23:33 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/17 23:37:33 by ehaanpaa         ###   ########.fr       */
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
	if (strncmp(argv[0], "-n", 3))
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

void print_export(void)
{
	char *s = "vegenakki";
	printf("%s\n", s);
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

static
void	wait_for_sub_processes(t_minishell *minishell)
{
	uint32_t	i;
	int			wstatus;
	pid_t		pid;

	i = 0;
	printf("command count: %o\n", minishell->command_count);
	while (i < minishell->command_count)
	{
		pid = wait(&wstatus);
		if (pid == (pid_t)(-1))
			perror("wait issue");
		if (pid == minishell->pids[minishell->command_count - 1])
		{
			printf("pid == %i\n", pid);
			minishell->exit_status = WEXITSTATUS(wstatus);
		}
		i++;
	}
}


void print_token(t_token *token)
{
	printf("token number: %i\ttoken name: %s\ttoken string: %.*s\n",
			token->type, token->name, (int)token->string_len, token->string);
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

void minishell_exec_loop(t_minishell *minishell, t_arena *arena, t_node *tree)
{
	t_node *head;
	char **envp = minishell->envp;
	int i;
	i = 0;

	head = tree;
	while (tree)
	{
		if (tree->token.type == PIPE)
		{
			int pipe_fds[2]; 
			pipe(pipe_fds);
			store_redirects(NULL , &pipe_fds[WRITE], minishell);
		}
		if (tree->token.type == WORD)
		{
			if (ft_strncmp("exit", tree->token.string, tree->token.string_len) == 0)
				return ;
			print_token(&tree->token);
		}
		else if(tree->token.type == HERE_DOCUMENT) // fully temp stuff
		{
			char *delimiter = calloc(1, tree->token.string_len + 1);
			ft_memmove(delimiter, tree->token.string, tree->token.string_len);
			heredoc(minishell, delimiter); // delimiter will still have quotes removed
			free(delimiter);
		}
		else
		{
			print_token(&tree->token);
		}
		tree = tree->left;
	}
	pid_t pid = fork();
	if (pid == 0)
	{
		apply_redirect(minishell);
		char *cat_argv[3] = {[0] = "cat", "-e", [2] = NULL}; //@TODO <- do this to tree and expand it
		printf("fd to cat \t%i\n", 17);
		if (execve("/bin/cat", cat_argv, envp) == -1)
			perror("execve fail");
		exit(1);
	}
	reset_redirect(minishell);
	minishell->pids = arena_alloc(arena, sizeof(minishell->pids));
	minishell->pids[minishell->command_count] = pid;
	minishell->command_count += 1;
	wait_for_sub_processes(minishell);
	printf("last printf %s\t%i\n", strerror(minishell->exit_status), minishell->exit_status);
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
			break ; // @TODO: error cheking
		add_history(line);
    	//printf("%s", line);
		m->line_counter += 1;
		t_node *tree = parser(&m->node_arena, line);
		if (tree)
			minishell_exec_loop(m, &m->node_arena, tree);
		arena_reset(&m->node_arena);
	}
}

void minishell_cleanup(t_minishell *minishell)
{
	arena_delete(&minishell->node_arena);
	arena_delete(&minishell->scratch_arena);
}
// set default values for the minishell struct
// the struct is going to work as a kind of storage for globally needef alues
// we might want to pre allocate the arenas that will be used here to make cleanup easier
void init_minishell(t_minishell *minishell, char **envp)
{
	minishell->command_count = 0;
	minishell->line_counter = 0;
	minishell->exit_status = 0;
	minishell->heredoc_count = 0;
	minishell->envp = envp;
	minishell->redir_fds[READ] = STDIN_FILENO;
	minishell->redir_fds[WRITE] = STDOUT_FILENO;
	minishell->pids = NULL;
	minishell->node_arena = arena_new(DEFAULT_ARENA_CAPACITY);
	if (minishell->node_arena.data == NULL)
		; //@TODO: error cheking
	minishell->scratch_arena = (t_arena){0};
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