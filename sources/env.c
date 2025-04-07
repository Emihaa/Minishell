/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 19:43:59 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/07 21:54:43 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// already done, is there more to do?
void print_env(char **env)
{
	int i;

	i = 0;
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
}

char **print_export(t_minishell *m)
{
	char **pntr_array;
	int i;

	i = 0;
	pntr_array = malloc(sizeof(char *) * (m->envp_size + 1));
	if (!pntr_array)
		return (NULL);
	while (i < m->envp_size)
	{
		pntr_array[i] = m->envp[i];
		i++;
	}
	pntr_array[i] = NULL;
	i = 0;
	// sort the pntr_array here
	// while (pntr_array[i])
	// {
		
	// 	i++;
	// }
	return (pntr_array);
}

char **create_env(char **envp, t_minishell *m)
{
	char **env_copy;
	int i;
	
	i = 0;
	while (envp[m->envp_size])
		m->envp_size++;
	env_copy = malloc (sizeof(char *) * (m->envp_size + 1));
	if (!env_copy)
		return (NULL);
	while (envp[i])
	{	
		env_copy[i] = ft_strdup(envp[i]);
		i++;
	}
	env_copy[i] = NULL;
	return (env_copy);
}
