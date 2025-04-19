/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 16:43:07 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/18 00:13:04 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// prints env in alphabetical order by creating new pntr_array
// and sorting the pointers
void	print_export(t_minishell *m)
{
	char	**pntr_array;
	int		i;

	i = 0;
	pntr_array = malloc(sizeof(char *) * (m->envp_size + 1));
	if (!pntr_array)
	{
		put_str(2, "Malloc failed\n");
		error_exit(m, 1);
	}
	while (i < m->envp_size)
	{
		pntr_array[i] = m->envp[i];
		i++;
	}
	pntr_array[i] = NULL;
	i = 0;
	env_sort(pntr_array, m->envp_size);
	print_export_env(pntr_array);
	free(pntr_array);
}

// check if there is match in the env and strdup on top of that the new info
// else see if there is empty space on the env, and strdup on the NULL point
// if not space then recreate the env with double the size
// @TODO _something doesnt show up on export but 
// should be the last one on env print?
int	export_add(t_minishell *m, char *s)
{
	int	pos;

	pos = get_env_key_index(s, get_key_len(s, ft_strlen(s)), m->envp);
	if (pos != -1)
	{
		free(m->envp[pos]);
		m->envp[pos] = ft_strdup(s);
		return (0);
	}
	pos = find_slot(m);
	if (pos == -1)
	{
		if (create_new_env(m, m->envp))
		{
			put_str(2, "Malloc failed\n");
			error_exit(m, 1);
		}
		return (0);
	}
	m->envp[pos] = ft_strdup(s);
	m->envp_size++;
	return (0);
}

void	builtin_export(t_minishell *m, int argc, char **argv)
{
	int	i;

	i = 1;
	if (argc == 1)
		print_export(m);
	else
	{
		while (i < argc)
		{
			if (spelling_check(m, argv[i]))
			{
				i++;
				continue ;
			}
			if (export_add(m, argv[i]))
				return ;
			i++;
		}
	}
}
