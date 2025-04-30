/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 17:41:23 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/28 17:54:02 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtin.h"

/// @brief return the pos of char if it is there in string
int	char_pos(char *s, char c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return (i);
		i++;
	}
	return (0);
}

/// @brief skip over the possible NULL strings, and then print
/// if size == 1 then the env[i] doesnt have '='
void	print_export_env(char **env)
{
	int		i;
	int		size;
	char	*temp;

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

void	swap_pntrs(char **p1, char **p2)
{
	char	*temp;

	temp = *p1;
	*p1 = *p2;
	*p2 = temp;
}

/// @brief checks if the env is sorted alphabetically returns 1 if true
int	is_sorted(char **pntr_array, int size)
{
	int	i;

	i = 0;
	while (i < size - 1)
	{
		if (ft_strncmp(pntr_array[i], pntr_array[i + 1],
				ft_strlen(pntr_array[i])) > 0)
			return (0);
		i++;
	}
	return (1);
}

/// @brief sorts the env in alphabetical order for export print
/// the strings should be compared until the end
void	env_sort(char **pntr_array, int size)
{
	int	i;

	while (1)
	{
		i = 0;
		while (i < size - 1)
		{
			if (ft_strncmp(pntr_array[i], pntr_array[i + 1],
					ft_strlen(pntr_array[i])) > 0)
			{
				swap_pntrs(&pntr_array[i], &pntr_array[i + 1]);
			}
			i++;
		}
		if (is_sorted(pntr_array, size))
			break ;
	}
}
