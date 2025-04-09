/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 19:43:59 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/09 19:58:35 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


// env is always double the size what i am copying
// for export space
char **create_env(char **envp, t_minishell *m)
{
	char **env_copy;
	int i;
	
	i = 0;
	while (envp[m->envp_size])
		m->envp_size++;
	env_copy = ft_calloc((m->envp_size * 2) + 1, sizeof(char *));
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
