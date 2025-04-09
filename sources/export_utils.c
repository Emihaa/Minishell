/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 17:41:23 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/09 22:33:29 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
		if (env[i] == NULL)
		{
			i++;
			continue ;
		}
		size = char_pos(env[i], '=') + 1;
		if (size == 1)
		{
			printf("declare -x %s\n", env[i]);
			i++;
			continue ;
		}
		temp = ft_strchr(env[i], '=') + 1;
		printf("declare -x %.*s\"%s\"\n", size, env[i], temp);
		i++;
	}
}

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
