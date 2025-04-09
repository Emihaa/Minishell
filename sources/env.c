/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 19:43:59 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/09 23:25:28 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// the og env is full, so we will create a new one with double the capacity
char **create_new_env(t_minishell *m , char **envp)
{
	char **env_copy;
	int i;
	
	i = 0;
	m->env_capacity = m->envp_size * 2;
	env_copy = ft_calloc(m->env_capacity, sizeof(char *));
	if (!env_copy)
		return (NULL);
	while (envp[i])
	{	
		env_copy[i] = envp[i];
		i++;
	}
	env_copy[i] = NULL;
	free(envp);
	printf("created new env!\n");
	return (env_copy);	
}

// env is always double the capacity what i am copying
// for export space
char **create_env(t_minishell *m, char **envp)
{
	char **env_copy;
	int i;
	
	i = 0;
	while (envp[m->envp_size])
		m->envp_size++;
	printf("env size %d\n", m->envp_size);
	m->env_capacity = m->envp_size * 2;
	env_copy = ft_calloc(m->env_capacity, sizeof(char *));
	if (!env_copy)
		return (NULL);
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
