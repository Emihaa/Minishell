/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 17:21:30 by ltaalas           #+#    #+#             */
/*   Updated: 2025/05/06 16:56:06 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtin.h"

/// @brief echo command
void	builtin_echo(char *argv[])
{
	bool	newline;
	int		i;

	if (argv[1] == NULL)
	{
		(void)put_char(STDOUT_FILENO, '\n');
		return ;
	}
	if (ft_strncmp(argv[1], "-n", 3) == 0)
		newline = false;
	else
		newline = true;
	i = !newline + 1;
	while (ft_strncmp(argv[i], "-n", 3) == 0)
		i++;
	while (1)
	{
		(void)put_str(STDOUT_FILENO, argv[i]);
		i++;
		if (argv[i] == NULL)
			break ;
		(void)put_char(STDOUT_FILENO, ' ');
	}
	if (newline == true)
		(void)put_char(STDOUT_FILENO, '\n');
}

/// @brief exit command
void	builtin_exit(t_minishell *m, int argc, char **argv)
{
	FILE	*temp;

	temp = stdout;
	if (m->pipe_side == -1)
		write_bytes(STDERR_FILENO, "exit\n", 5);
	if (argv != NULL)
	{
		if (argc > 1 && str_is_numeric(argv[1]) == false)
		{
			stdout = stderr;
			printf("minishell: exit: %s: numeric argument required\n", argv[1]);
			stdout = temp;
			m->exit_status = 2;
		}
		else if (argc == 2)
			m->exit_status = ft_atoi(argv[1]);
		else if (argc > 2)
		{
			write_bytes(2, "minishell: exit: too many arguments\n", 36);
			m->exit_status = 1;
			return ;
		}
	}
	minishell_cleanup(m);
	exit(m->exit_status);
}

/// @brief pwd command
void	builtin_pwd(t_minishell *m)
{
	char	current_path[PATH_MAX];

	if (getcwd(current_path, PATH_MAX) == NULL)
	{
		m->exit_status = 1;
		perror("minishell: pwd");
		return ;
	}
	(void)put_str_nl(STDOUT_FILENO, current_path);
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
		builtin_echo(argv);
	if (command == BUILTIN_CD)
		builtin_cd(m, count_argc(argv), argv);
	if (command == BUILTIN_PWD)
		builtin_pwd(m);
	if (command == BUILTIN_ENV)
		builtin_env(m->envp);
	if (command == BUILTIN_UNSET)
		builtin_unset(m, count_argc(argv), argv);
	if (command == BUILTIN_EXPORT)
		builtin_export(m, count_argc(argv), argv);
	return (0);
}
