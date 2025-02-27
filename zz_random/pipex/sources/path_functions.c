/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 21:15:36 by ltaalas           #+#    #+#             */
/*   Updated: 2025/02/23 17:48:23 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

char	*get_bin_paths(char **envp)
{
	int		i;
	char	*path_line;

	i = 0;
	path_line = NULL;
	if (envp == NULL)
		return (NULL);
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_line = envp[i];
			path_line += 5;
			return (path_line);
		}
		i++;
	}
	return (NULL);
}

uint32_t	bin_path_len(const char *bin_paths)
{
	int	i;

	i = 0;
	if (bin_paths == NULL)
		return (0);
	while (bin_paths[i] && bin_paths[i] != ':')
		i++;
	return (i);
}

char	*get_cmd_with_path(char **cmd_argv, const char *bin_paths)
{
	const uint32_t	cmd_str_siz = ft_strlen(cmd_argv[0]);
	const uint32_t	paths_len = ft_strlen(bin_paths);
	char			*cmd_with_path;
	uint32_t		path_len;
	uint32_t		i;

	i = 0;
	while (i < paths_len)
	{
		path_len = bin_path_len(&bin_paths[i]);
		cmd_with_path = ft_calloc_fastish(cmd_str_siz + path_len + 2,
				sizeof(char));
		if (cmd_with_path == NULL)
			error_malloc(cmd_argv);
		ft_memmove_fastish(cmd_with_path, &bin_paths[i], path_len);
		cmd_with_path[path_len] = '/';
		ft_memmove_fastish(&cmd_with_path[path_len + 1], cmd_argv[0],
			cmd_str_siz);
		if (access(cmd_with_path, F_OK) == 0)
			return (cmd_with_path);
		free(cmd_with_path);
		cmd_with_path = NULL;
		i += path_len + 1;
	}
	return (NULL);
}

char	*check_for_cmd_path(char *cmd_str, char **cmd_argv, char **envp)
{
	const char	*bin_paths = get_bin_paths(envp);
	char		*cmd_with_path;

	if (cmd_argv[0] == NULL)
	{
		if (bin_paths != NULL && bin_paths[0] != '\0')
			error_command_not_found(cmd_argv, NULL, cmd_str);
		cmd_with_path = ft_strdup(cmd_str);
		if (cmd_with_path == NULL)
			error_malloc(cmd_argv);
		if (bin_paths == NULL || bin_paths[0] == '\0')
			return (cmd_with_path);
	}
	if (!strchr(cmd_argv[0], '/') && bin_paths != NULL && bin_paths[0] != '\0')
	{
		cmd_with_path = get_cmd_with_path(cmd_argv, bin_paths);
		if (cmd_with_path == NULL || is_directory(cmd_with_path))
			error_command_not_found(cmd_argv, cmd_with_path, cmd_argv[0]);
	}
	else
		cmd_with_path = cmd_argv[0];
	return (cmd_with_path);
}
