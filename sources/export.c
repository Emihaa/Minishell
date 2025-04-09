/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 16:43:07 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/09 21:23:04 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

static 
char **find_slot(t_minishell *m, int *pos)
{
	int i;

	i = 0;
	while (i <= m->envp_size * 2)
	{
		if (m->envp[i] == NULL)
		{
			pos = &i;
			printf("found empty slot on eval\n");
			return (&m->envp[i]);
		}
		i++;
	}
	return (NULL);
}

static 
char **find_match(t_minishell *m, char *s)
{
	int i;
	int size;
	
	i = 0;
	size = 0;
	while (s[size] != '=' && s[size] != '\0')
		size++;
	while (i <= (m->envp_size * 2))
	{
		if (m->envp[i] == NULL)
		{
			i++;
			continue ;
		}
		if (ft_strncmp(s, m->envp[i], size) == 0)
		{
			printf("match found!\n");
			return (&m->envp[i]);
		}
		i++;
	}
	return (NULL);	
}

// find the first empty slot or find a match to overwrite the previous
// first tries to find the first empty slot in the env and add the new info there.
// else put the new info to the bottom of the list
// order of the env is undefined

// everything after export goes to the new env to the bottom of the list??
// second last because i want the _something to be the last
// or what is the order?
// if the export has '=' then 'env' will print it out
void cmd_export(t_minishell *m, int argc, char **argv)
{
	// first argument is always the "export"? right? yes
	int i;
	int pos;
	char **slot;
	// char *temp;

	i = 1;
	slot = NULL;
	while (i < argc)
	{
		pos = 0;
		slot = find_match(m, argv[i]);
		if (slot)
		{
			*slot = ft_strdup(argv[i]);
			printf("%s\n", *slot);
			m->envp_size++;
			i++;
			continue ;
		}
		slot = find_slot(m, &pos);
		if (!slot)
		{	
			// redo the env with double the size
			return ;
		}
		*slot = ft_strdup(argv[i]);
		printf("%s\n", *slot);
		m->envp_size++;
		i++;
	}	
}

void builtin_export(t_minishell *m, int argc, char **argv)
{
	(void) argv;
	if (argc == 1)
		print_export(m);
	else
		cmd_export(m, argc, argv);
}