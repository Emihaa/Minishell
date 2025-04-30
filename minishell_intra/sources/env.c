/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 19:43:59 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/25 17:26:09 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtin.h"

/// @brief the og env is full, create a new one with double the capacity
int	create_new_env(t_minishell *m, char **envp)
{
	char	**env_copy;
	int		i;

	i = 0;
	env_copy = ft_calloc((m->env_capacity * 2) + 1, sizeof(char *));
	if (!env_copy)
		return (1);
	while (i < m->env_capacity)
	{
		env_copy[i] = envp[i];
		i++;
	}
	m->env_capacity = m->env_capacity * 2;
	env_copy[i] = NULL;
	free(envp);
	m->envp = env_copy;
	return (0);
}

/// @brief check if '=' on env and print that line otherwise skip
void	builtin_env(char **envp)
{
	int	i;

	i = 0;
	if (envp == NULL)
		return ;
	while (envp[i])
	{
		if (ft_strchr(envp[i], '='))
			printf("%s\n", envp[i]);
		i += 1;
	}
}

/// @brief env is always double the capacity, of what is copied
/// in the beginning
char	**create_env(t_minishell *m, char **envp)
{
	char	**env_copy;
	int		i;

	i = 0;
	while (envp[m->envp_size])
		m->envp_size++;
	if (m->envp_size >= m->env_capacity)
		m->env_capacity = (m->envp_size * 2);
	env_copy = ft_calloc(m->env_capacity + 1, sizeof(char *));
	if (!env_copy)
	{
		return (NULL);
	}
	while (envp[i])
	{
		env_copy[i] = ft_strdup(envp[i]);
		if (!env_copy[i])
			return (NULL);
		i++;
	}
	env_copy[i] = NULL;
	return (env_copy);
}
