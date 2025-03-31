/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 17:21:30 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/31 22:54:29 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void builtin_exit(t_minishell *m)
{
	minishell_cleanup(m);
	if (m->pipe_side != -1)
		write(2, "exit\n", 5);
	exit(m->exit_status);
}

//pwd command
//propably need file dupping (dup2)
void	builtin_pwd(void)
{
	char current_path[PATH_MAX];
	getcwd(current_path, PATH_MAX);
	printf("%s\n", current_path);
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
		builtin_exit(m);
	if (command == BUILTIN_ECHO)
		builtin_echo(argv); // @TODO: add command
	if (command == BUILTIN_CD)
		; // @TODO: add command
	if (command == BUILTIN_PWD)
		builtin_pwd(); // @TODO: add command
	if (command == BUILTIN_ENV)
		; // @TODO: add command
	if (command == BUILTIN_UNSET)
		; // @TODO: add command
	if (command == BUILTIN_EXPORT)
		; // @TODO: add command
	return (0);
}