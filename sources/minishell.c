/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:23:33 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/02 19:34:35 by ltaalas          ###   ########.fr       */
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

void read_loop(char **envp)
{
	char *line;
	while (1)
	{
		line = readline("minishell> ");
		add_history(line);
    	//printf("%s", line);
		if (strncmp(line, "env", 4) == 0)
		{
			env(envp);
			continue;
		}
		if (strncmp(line, "pwd", 4) == 0)
		{
			print_working_directory();
			continue;
		}
		if (strncmp(line, "export", 4) == 0)
		{
			print_export();
			continue;
		}
		if (*line == '$')
		{
			print_expansion(line);
			continue;
		}
		if (strncmp(line, "exit", 5) == 0)
			return ;
	}
}

int main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	read_loop(envp);
	printf("exit\n");
    return (0);
}