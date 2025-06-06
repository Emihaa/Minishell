/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 21:17:54 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/25 17:24:32 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtin.h"

/// @brief first letter of env variable can only be "_" or alpha
inline
bool	is_valid_var_start(char c)
{
	return (ft_isalpha(c) || c == '_');
}

uint32_t	get_env_key_len(char *src, uint32_t src_len)
{
	uint32_t	i;

	i = 0;
	while (i < src_len)
	{
		if (src[i] != '_' && ft_isalnum(src[i]) == false)
			break ;
		i += 1;
	}
	return (i);
}

/// @brief compares for a str match from env and returns index pos
/// of a match if tru, else -1 
int	get_env_key_index(char *key, uint32_t key_len, char **envp)
{
	int	i;

	i = 0;
	if (key_len > 0)
	{
		while (envp[i] != NULL)
		{
			if (ft_strncmp(key, envp[i], key_len) == 0
				&& (envp[i][key_len] == '=' || envp[i][key_len] == '\0'))
			{
				return (i);
			}
			i += 1;
		}
	}
	return (-1);
}

/// @brief returns address of string after "=" if there is a match
/// if string doesn't have variable returns NULL
char	*get_env_var_value(char *key, uint32_t key_len)
{
	int		index;
	char	**envp;

	envp = get_minishell(NULL)->envp;
	if (envp == NULL)
		return (NULL);
	index = get_env_key_index(key, key_len, envp);
	if (index < 0)
		return (NULL);
	if (envp[index][key_len] == '\0' || envp[index][key_len + 1] == '\0')
		return (NULL);
	return (&envp[index][key_len + 1]);
}
