/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:23:33 by ltaalas           #+#    #+#             */
/*   Updated: 2025/02/27 22:32:45 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <linux/limits.h>


//pwd command
//propably need file dupping (dup2)
void print_working_directory(void)
{
	char current_path[PATH_MAX];
	getcwd(current_path, PATH_MAX);
	printf("pwd: %s\n", current_path); // 
}

void read_loop(void)
{
	char *line;
	while (1)
	{
		line = readline("minishell >");
    	printf("%s\n", line);
		if (strncmp(line, "exit", 4) == 0)
			return ;
	}
}

int main(void)
{
	//read_loop();
	print_working_directory();
	printf("exit");
    return (0);
}