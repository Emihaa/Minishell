/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:23:33 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/09 22:45:36 by ltaalas          ###   ########.fr       */
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

static int	wait_for_sub_processes(t_minishell *minishell)
{
	uint32_t	i;
	int			wstatus;
	int			exit_code;
	pid_t		pid;

	i = 0;
	exit_code = 1;
	printf("command count: %o\n", minishell->command_count);
	while (i < minishell->command_count)
	{
		pid = wait(&wstatus);
		if (pid == (pid_t)(-1))
			perror("wait issue");
		if (pid == minishell->pids[minishell->command_count - 1])
		{
			printf("pid == %i\n", pid);
			exit_code = WEXITSTATUS(wstatus);
		}
		i++;
	}
	return (exit_code);
}
void read_loop(char **envp, t_minishell *minishell)
{
	char *line;
	t_arena token_array_arena = arena_new(DEFAULT_ARENA_CAPACITY);
	t_token *token_array;
	t_lexer lexer;
	while (1)
	{
		minishell->command_count = 0;
		line = readline("minishell> ");
		if (line == NULL)
			break ; // @TODO: error cheking
		add_history(line);
    	//printf("%s", line);
		minishell->line_counter += 1;
		lexer.line = line;
		lexer.line_index = 0;
		token_array = get_token_array(&token_array_arena, &lexer);
		for (int i = 0; token_array[i].type != END_OF_LINE; ++i)
		{
			if (token_array[i].type == WORD)
			{
				if (ft_strncmp("exit", token_array[i].string, token_array[i].string_len) == 0)
					break;
			}
			else if(token_array[i].type == HERE_DOCUMENT)
			{
				char *delimiter = calloc(1, token_array[i].string_len + 1);
				ft_memmove(delimiter, token_array[i].string, token_array[i].string_len);
				heredoc(minishell, delimiter);
				free(delimiter);
			}
			else
			{
				char *token_string = calloc(1, token_array[i].string_len + 1);
				ft_memmove(token_string, token_array[i].string, token_array[i].string_len);
				printf("token number: %i\ttoken name: %s\ttoken string: %s\n",
						token_array[i].type, token_array[i].name, token_string);
				free(token_string);
			}
		}
		pid_t pid = fork();
		if (pid == 0)
		{
			apply_redirect(minishell);
			char *cat_argv[3] = {[0] = "cat", "-e", [2] = NULL};
			printf("fd to cat \t%i\n", 17);
			if (execve("/bin/cat", cat_argv, envp) == -1)
				perror("execve fail");
			exit(1);
		}
		reset_redirect(minishell);
		minishell->pids[minishell->command_count] = pid;
		minishell->command_count += 1;
		minishell->exit_status = wait_for_sub_processes(minishell);
		printf("last printf %s\t%i\n", strerror(minishell->exit_status), minishell->exit_status);
		arena_reset(&token_array_arena);
	}
}



void init_minishell(t_minishell *minishell)
{
	minishell->command_count = 0;
	minishell->line_counter = 0;
	minishell->exit_status = 0;
	minishell->heredoc_count = 0;
	minishell->redir_fds[READ] = STDIN_FILENO;
	minishell->redir_fds[WRITE] = STDOUT_FILENO;
	minishell->pids = malloc(sizeof(*minishell->pids) * 2);
}

int main(int argc, char *argv[], char *envp[])
{
	t_minishell minishell;
	(void)argc;
	(void)argv;
	init_minishell(&minishell);
	read_loop(envp, &minishell);
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