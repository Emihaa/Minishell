/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 17:59:21 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/17 17:49:49 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// @TODO: exit values

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
	temp = xarena_alloc(&m->node_arena, len1 + len2 + 1);
	if (s1)
		ft_memmove(temp, s1, len1);
	if (s2)
		ft_memmove(&temp[len1], s2, len2);
	temp[len1 + len2 + 1] = 0;
	return (temp);
}

static void	cd_home(t_minishell *m)
{
	char	*temp;
	int		pos;

	pos = check_match(m, "HOME");
	if (pos == -1)
	{
		put_str(2, "minishell: cd: HOME not set\n"); // exit value has to be 1
		return ;
	}
	temp = arena_strjoin(m, NULL, ft_strchr(m->envp[pos], '=') + 1);
	chdir(temp);
}

static void	update_pwd(t_minishell *m, char *old_path)
{
	char	current_path[PATH_MAX];
	char	*temp;

	temp = arena_strjoin(m, "OLDPWD=", old_path);
	export_add(m, temp);
	if (getcwd(current_path, PATH_MAX) == NULL)
	{
		put_str(2, "minishell: cd: pwd NULL\n"); // exit status 1
		return ;
	}
	temp = arena_strjoin(m, "PWD=", current_path);
	export_add(m, temp);
}

// changes directory based on input
// argc == 1 or argc == 2 changes to the given folder
// if more arguments writes error
void	builtin_cd(t_minishell *m, int argc, char **argv)
{
	FILE	*temp;
	char	current_path[PATH_MAX];

	temp = stdout;
	if (argc == 1 || argc == 2)
	{
		if (getcwd(current_path, PATH_MAX) == NULL)
		{
			put_str(2, "minishell: cd: pwd NULL\n"); // exit status 1
			return ;
		}
		if (argc == 1)
			cd_home(m);
		if (argc != 1 && chdir(argv[1]) == -1)
		{
			stdout = stderr;
			printf("minishell: cd: %s: No such file or directory\n", argv[1]); // exit status 1
			stdout = temp;
			return ;
		}
		update_pwd(m, current_path);
	}
	else if (argc > 2)
		put_str(2, "minishell: cd: too many arguments\n"); // exit status 1
}
