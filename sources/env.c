/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 19:43:59 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/11 22:13:31 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static
int	strncmp_to_c(const char *s1, const char *s2, char c, size_t n)
{
	int	i;

	i = 0;
	while (n-- != 0)
	{
		if (s1[i] == c)
			return (1);
		if (s1[i] != s2[i])
			return (1);
		if (s1[i] == '\0')
			return (0);
		i++;
	}
	return (0);
}

// static 
// char **find_match(t_minishell *m, char *s)
// {
// 	int i;
// 	int size;	
// 	i = 0;
// 	size = 0;
// 	while (s[size] != '\0')
// 		size++;
// 	while (i < m->env_capacity)
// 	{
// 		if (m->envp[i] == NULL)
// 		{
// 			i++;
// 			continue ;
// 		}
// 		if (strncmp_to_c(s, m->envp[i], '=', size) == 0 && \
// 			(m->envp[i][size] == '\0' || m->envp[i][size] == '='))
// 			return (&m->envp[i]);
// 		i++;
// 	}
// 	return (NULL);	
// }

static 
int find_match(t_minishell *m, char *s)
{
	int i;
	int size;
	
	i = 0;
	size = 0;
	while (s[size] != '\0')
		size++;
	while (i < m->env_capacity)
	{
		if (m->envp[i] == NULL)
		{
			i++;
			continue ;
		}
		if (strncmp_to_c(s, m->envp[i], '=', size) == 0 && \
			(m->envp[i][size] == '\0' || m->envp[i][size] == '='))
			return (i);
		i++;
	}
	return (-1);	
}

void builtin_unset(t_minishell *m, int argc, char **argv)
{
	int pos;
	int i;

	pos = 0;
	i = 1;
	while (i < argc)
	{
		pos = find_match(m, argv[i]);
		if (pos != -1)
		{
			printf("unset: %s\n", m->envp[pos]);
			free(m->envp[pos]);
			m->envp[pos] = NULL;
			m->envp_size--;
		}
		i++;
	}
}


// the og env is full, so we will create a new one with double the capacity
int create_new_env(t_minishell *m , char **envp)
{
	char **env_copy;
	int i;
	
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
	printf("created new env!\n");
	return (0);	
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
	if (m->envp_size >= m->env_capacity)
		m->env_capacity = (m->envp_size * 2);
	printf("env size %d env capacity %d\n", m->envp_size, m->env_capacity);
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
