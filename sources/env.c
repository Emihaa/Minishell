/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 19:43:59 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/08 00:26:59 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static
int char_pos(char *s, char c)
{
	int i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return(i);
		i++;
	}
	return (0);
}

static
void print_export_env(char **env)
{
	int i;
	int size;
	char *temp;

	i = 0;
	size = 0;
	temp = NULL;
	while (env[i])
	{
		size = char_pos(env[i], '=') + 1;
		temp = ft_strchr(env[i], '=') + 1;
		printf("declare -x %.*s\"%s\"\n", size, env[i], temp);
		i++;
	}
}

static
void swap_pntrs(char **p1, char **p2)
{
	char *temp;

	temp = *p1;
	*p1 = *p2;
	*p2 = temp;
}

int is_sorted(char **pntr_array, int size)
{
	int i;

	i = 0;
	while (i < size - 1)
	{
		if (ft_strncmp(pntr_array[i], pntr_array[i + 1], 1024) > 0)
			return (0);
		i++;
	}
	return (1);
}

void env_sort(char **pntr_array, int size)
{
	int i;

	while (1)
	{
		i = 0;
		while (i < size - 1)
		{
			if (ft_strncmp(pntr_array[i], pntr_array[i + 1], 1024) > 0)
			{
				swap_pntrs(&pntr_array[i], &pntr_array[i + 1]);
			}
			i++;
		}
		if (is_sorted(pntr_array, size))
			break;
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
	env_sort(pntr_array, m->envp_size);
	print_export_env(pntr_array);
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
