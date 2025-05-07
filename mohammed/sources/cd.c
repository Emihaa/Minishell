/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 17:59:21 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/05/07 23:43:28 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtin.h"

/// @brief custom strjoin for arena usage
static char	*arena_strjoin(t_minishell *m, char *s1, char *s2)
{
	char	*temp;
	int		len1;
	int		len2;

	len1 = 0;
	len2 = 0;
	if (s1)
		len1 = ft_strlen(s1);
	if (s2)
		len2 = ft_strlen(s2);
	temp = xarena_alloc(m->global_arena, len1 + len2 + 1);
	if (s1)
		ft_memmove(temp, s1, len1);
	if (s2)
		ft_memmove(&temp[len1], s2, len2);
	temp[len1 + len2] = 0;
	return (temp);
}

/// @brief changes directory to path at "HOME"
static int	cd_home(t_minishell *m)
{
	FILE	*file;
	char	*temp;
	int		pos;

	pos = get_env_key_index("HOME", 4, m->envp);
	if (pos == -1)
	{
		put_str(2, "minishell: cd: HOME not set\n");
		m->exit_status = 1;
		return (1);
	}
	temp = arena_strjoin(m, NULL, ft_strchr(m->envp[pos], '=') + 1);
	if (chdir(temp) == -1)
	{
		file = stdout;
		stdout = stderr;
		printf("minishell: cd: %s: %s\n", temp, strerror(errno));
		m->exit_status = 1;
		stdout = file;
		return (1);
	}
	return (0);
}

/// @brief updates the "OLDPWD" & "PWD" in the env
static void	update_pwd(t_minishell *m)
{
	char	current_path[PATH_MAX];
	char	*temp;

	temp = arena_strjoin(m, "OLDPWD=", get_env_var_value("PWD", 3));
	export_add(m, temp);
	if (getcwd(current_path, PATH_MAX) == NULL)
	{
		perror("minishell: cd: getcwd");
		m->exit_status = 1;
		return ;
	}
	temp = arena_strjoin(m, "PWD=", current_path);
	export_add(m, temp);
}

/// @brief changes directory based on input
/// argc == 1 or argc == 2 changes to the given folder
/// if more arguments, writes error
void	builtin_cd(t_minishell *m, int argc, char **argv)
{
	FILE	*temp;

	temp = stdout;
	if (argc == 1 || argc == 2)
	{
		if (argc == 1)
			if (cd_home(m))
				return ;
		else if (chdir(argv[1]) == -1)
		{
			stdout = stderr;
			printf("minishell: cd: %s: %s\n", argv[1], strerror(errno));
			m->exit_status = 1;
			stdout = temp;
			return ;
		}
		update_pwd(m);
	}
	if (argc > 2)
	{
		put_str(2, "minishell: cd: too many arguments\n");
		m->exit_status = 1;
	}
}
