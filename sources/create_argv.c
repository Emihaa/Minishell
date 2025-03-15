/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_argv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:12:30 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/15 19:01:02 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


// this is just a new quick and dirty thing
// will be completely different once we have enough data

// PLEASE give a completely new and unique arena used just for this argv
char **start_command_argv(t_arena *a, t_token *data)
{
	
	const char **r_val = arena_alloc(a, sizeof(char *) * 2);
	r_val[0] = ft_calloc(data->string_len + 1, sizeof(char));
	if (r_val[0] == NULL)
		return (NULL);
	ft_memmove(&r_val[0], data->string, data->string_len);
	return (r_val);
}

// give same arena as for the start
char **add_to_command_argv(const char **argv, t_arena *a, t_token *data)
{
	int i;

	i = 0;
	while (argv[i] != NULL)
		i++;
	arena_alloc(a, sizeof(char *) * 1);
	argv[i] = ft_calloc(data->string_len + 1, sizeof(char));
	if (argv[i] == NULL)
		return (NULL);
	ft_memmove(&argv[i], data->string, data->string_len);
	return (argv);
}	