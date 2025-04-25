/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 18:45:02 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/25 01:00:56 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtin.h"

// compare the string(s1) to env names(s2), if s1 has '=' char then return 1,
// as env names can't have '=' letters in names.
// otherwise if there are differences return 1 or if match then 0
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

// Find name match from the env and return the position of match in the env
static
int	find_match(t_minishell *m, char *s)
{
	int	i;
	int	size;

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
		if (strncmp_to_c(s, m->envp[i], '=', size) == 0
			&& (m->envp[i][size] == '\0' || m->envp[i][size] == '='))
			return (i);
		i++;
	}
	return (-1);
}

void	builtin_unset(t_minishell *m, int argc, char **argv)
{
	int	pos;
	int	i;

	pos = 0;
	i = 1;
	while (i < argc)
	{
		pos = find_match(m, argv[i]);
		if (pos != -1)
		{
			free(m->envp[pos]);
			m->envp[pos] = m->envp[m->envp_size - 1];
			m->envp[m->envp_size - 1] = NULL;
			m->envp_size--;
		}
		i++;
	}
}
