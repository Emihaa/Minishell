/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:23:33 by ltaalas           #+#    #+#             */
/*   Updated: 2025/02/27 19:53:28 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
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
	char *pwd;
    char buf[1024];
    getcwd(buf, 1024);
    printf("pwd: %s\n", pwd);
	printf("pwd: %s\n", buf);
	//read_loop();
	printf("exit");
    return (0);
}