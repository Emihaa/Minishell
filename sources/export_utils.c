/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 20:29:28 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/17 23:03:48 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// checkout for name matches from env for export, if found return the pos
// number from the env
// @TODO REMOVE THIS
// int	check_match(t_minishell *m, char *s)
// {
// 	int	i;
// 	int	size;

// 	i = 0;
// 	size = 0;
// 	while (s[size] != '=' && s[size] != '\0')
// 		size++;
// 	while (i < m->env_capacity)
// 	{
// 		if (m->envp[i] == NULL)
// 		{
// 			i++;
// 			continue ;
// 		}
// 		if (ft_strncmp(s, m->envp[i], size) == 0 && \
// 		(m->envp[i][size] == '\0' || m->envp[i][size] == '='))
// 			return (i);
// 		i++;
// 	}
// 	return (-1);
// }

// find empthy slot from the env and return the pos number
int	find_slot(t_minishell *m)
{
	int	i;

	i = 0;
	while (i <= m->env_capacity)
	{
		if (m->envp[i] == NULL)
		{
			return (i);
		}
		i++;
	}
	return (-1);
}

// check the spelling in the given export argument
// if the first letter is not alpha or '_' or
// if the rest of the letters are not numbers, alpha or '_'
int	spelling_check(t_minishell *m, char *s)
{
	int	i;

	i = 0;
	if (s[i] == '=')
	{
		printf("export: `%s': not a valid identifier\n", s);
		m->exit_status = 1;
		return (1);
	}
	while (s[i] != '\0' && s[i] != '=')
	{	
		if ((i == 0 && !ft_isalpha(s[i]) && s[i] != '_') || \
		(i != 0 && !ft_isalnum(s[i]) && s[i] != '_'))
		{
			printf("export: `%s': not a valid identifier\n", s); //@TODO standar error
			m->exit_status = 1;
			return (1);
		}
		i++;
	}
	return (0);
}
