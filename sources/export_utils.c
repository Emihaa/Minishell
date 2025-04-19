/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 20:29:28 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/19 18:50:55 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

static
void	error_print(t_minishell *m, char *s)
{
	FILE	*temp;

	temp = stdout;
	stdout = stderr;
	printf("minishell: export: `%s': not a valid identifier\n", s);
	m->exit_status = 1;
	stdout = temp;
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
		error_print(m, s);
		return (1);
	}
	while (s[i] != '\0' && s[i] != '=')
	{
		if ((i == 0 && !ft_isalpha(s[i]) && s[i] != '_') || \
		(i != 0 && !ft_isalnum(s[i]) && s[i] != '_'))
		{
			error_print(m, s);
			return (1);
		}
		i++;
	}
	return (0);
}
