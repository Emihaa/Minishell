/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:23:33 by ltaalas           #+#    #+#             */
/*   Updated: 2025/02/28 18:53:36 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <linux/limits.h>


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
	printf("pwd: %s\n", current_path); // 
}

void read_loop(char **envp)
{
	char *line;
	while (1)
	{
		line = readline("minishell> ");
    	//printf("%s\n", line);
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
		if (strncmp(line, "exit", 5) == 0)
			return ;
	}
}

int main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	read_loop(envp);
	printf("exit");
    return (0);
}