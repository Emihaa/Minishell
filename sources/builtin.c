/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 17:21:30 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/02 17:28:32 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// @TODO: we need to figure out how we want to do file redirections for builtins when run in the main process (parent)
// one way would be to not redirect anything but just write into the fd that minishell->fds[WRITE] points to
void builtin_echo(char *argv[], int fd)
{
	bool newline;
	int i;

	if (argv[1] == NULL)
	{
		write(1, "\n", fd);
		return ;
	}
	if (ft_strncmp(argv[1], "-n", 3) == 0)
		newline = false;
	else
		newline = true;
	i = !newline + 1;
	while (1)
	{
		ft_putstr_fd(argv[i], fd);
		i++;
		if (argv[i] == NULL)
			break ;
		ft_putchar_fd(' ', fd);
	}
	if (newline == true)
		ft_putchar_fd('\n', fd);
}

void builtin_exit(t_minishell *m)
{
	minishell_cleanup(m);
	if (m->pipe_side == -1)
		write(2, "exit\n", 5);
	exit(m->exit_status);
}

//pwd command
//propably need file dupping (dup2)
void	builtin_pwd(int fd)
{
	char current_path[PATH_MAX];
	getcwd(current_path, PATH_MAX);
	ft_putendl_fd(current_path, fd);
}

void builtin_env(char **envp)
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

// watch the youtube video that had something about path name shortening
// it also had some stuff about arenas and macros for c. was about 13mins long
// will need that path name shortening for this cd probably
// void builtin_cd(char **argv, char **env)
// {

// 	int i;
// 	const char *home = find_env_var("HOME", 4, i, env);
// 	char *curpath;
// 	char *directory;


// 	i = 0;
// 	while (argv[i] != NULL)
// 		i++;
// 	if (i > 1)
// 	{
// 		ft_putendl_fd("minishell: cd: too many arguments", 2);
// 		return (1);
// 	}
// 	directory = argv[0];
// 	if (argv[0] == NULL)
// 	{
// 		if (home == NULL)
// 		{

// 			ft_putendl_fd("minishell: cd: HOME not set", 2);
// 			return(1);
// 		}
// 		directory = home;
// 	}
// 	if (directory[0] == '/') // step 3
// 	{
// 		curpath = directory;
		
// 	}
// 	(directory[0] == '.')
// 	chdir()
// }


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
		builtin_echo(argv, m->redir_fds[WRITE]); // @TODO: add command
	if (command == BUILTIN_CD)
		; // @TODO: add command
	if (command == BUILTIN_PWD)
		builtin_pwd(m->redir_fds[WRITE]); // @TODO: add command
	if (command == BUILTIN_ENV)
		builtin_env(m->envp); // @TODO: add command
	if (command == BUILTIN_UNSET)
		; // @TODO: add command
	if (command == BUILTIN_EXPORT)
		; // @TODO: add command
	return (0);
}