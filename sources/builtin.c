/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 17:21:30 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/25 00:59:57 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtin.h"

// @TODO: we need to figure out how we want to do file redirections
// for builtins when run in the main process (parent)
// one way would be to not redirect anything but just write into the fd that
// minishell->fds[WRITE] points to
void	builtin_echo(char *argv[], int fd)
{
	bool	newline;
	int		i;

	if (argv[1] == NULL)
	{
		(void)put_char(fd, '\n');
		return ;
	}
	if (ft_strncmp(argv[1], "-n", 3) == 0)
		newline = false;
	else
		newline = true;
	i = !newline + 1;
	while (1)
	{
		(void)put_str(fd, argv[i]);
		i++;
		if (argv[i] == NULL)
			break ;
		(void)put_char(fd, ' ');
	}
	if (newline == true)
		(void)put_char(fd, '\n');
}

// should it have int argc instead of count_argc function?
// add it to the execute_builtin?
// or is this function called elsewhere as well?
void	builtin_exit(t_minishell *m, int argc, char **argv)
{
	if (m->pipe_side == -1)
		write_bytes(STDERR_FILENO, "exit\n", 5);
	if (argv != NULL)
	{
		if (argc > 1 && str_is_numeric(argv[1]) == false)
		{
			stdout = stderr;
			printf("minishell: exit: %s: numeric argument required\n", argv[1]);
			m->exit_status = 2;
		}
		else if (argc == 2)
		{
			m->exit_status = ft_atoi(argv[1]);
		}
		else if (argc > 2)
		{
			write_bytes(STDERR_FILENO,
				"minishell: exit: too many arguments\n", 36);
			m->exit_status = 1;
			return ;
		}
	}
	minishell_cleanup(m);
	exit(m->exit_status);
}

//pwd command
//propably need file dupping (dup2)
void	builtin_pwd(t_minishell *m, int fd)
{
	char	current_path[PATH_MAX];

	if (getcwd(current_path, PATH_MAX) == NULL)
	{
		m->exit_status = 1;
		perror("minishell: pwd:");
		return ;
	}
	(void)put_str_nl(fd, current_path);
}

t_builtin	check_for_builtin(char *command)
{
	if (ft_strncmp(command, "exit", 5) == 0)
		return (BUILTIN_EXIT);
	if (ft_strncmp(command, "echo", 5) == 0)
		return (BUILTIN_ECHO);
	if (ft_strncmp(command, "cd", 3) == 0)
		return (BUILTIN_CD);
	if (ft_strncmp(command, "pwd", 4) == 0)
		return (BUILTIN_PWD);
	if (ft_strncmp(command, "env", 4) == 0)
		return (BUILTIN_ENV);
	if (ft_strncmp(command, "unset", 5) == 0)
		return (BUILTIN_UNSET);
	if (ft_strncmp(command, "export", 5) == 0)
		return (BUILTIN_EXPORT);
	return (BUILTIN_FALSE);
}

int	execute_builtin(t_minishell *m, char **argv, t_builtin command)
{
	m->exit_status = EXIT_SUCCESS;
	if (command == BUILTIN_EXIT)
		builtin_exit(m, count_argc(argv), argv);
	if (command == BUILTIN_ECHO)
		builtin_echo(argv, m->redir_fds[WRITE]);
	if (command == BUILTIN_CD)
		builtin_cd(m, count_argc(argv), argv);
	if (command == BUILTIN_PWD)
		builtin_pwd(m, m->redir_fds[WRITE]);
	if (command == BUILTIN_ENV)
		builtin_env(m->envp);
	if (command == BUILTIN_UNSET)
		builtin_unset(m, count_argc(argv), argv);
	if (command == BUILTIN_EXPORT)
		builtin_export(m, count_argc(argv), argv);
	return (0);
}
