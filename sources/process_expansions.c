/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_expansions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 18:41:46 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/11 20:43:24 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


// crating a prototype thing
// again will most likely be completely remade once I know what to do better

static inline
int	match_double_quote(t_arena *a, t_token *data, int index)
{
	char	c;

	while (index < data->string_len)
	{
		c = data->string[index];
		if (c == '"' || c == '$')
			break ;
		index += 1;
	}
	if (c == '$')
		expand_variable(a, data, index);
	return (index);
}

int expand_variable(t_arena *a, t_token *data, int index)
{
	const t_arena_temp temp = arena_temp_begin(a);
	int		i;
	char	c;
	char	*var;

	i = 0;
	while (index < data->string_len)
	{
		c = data->string[index + i];
		if (c == '"' || c == '\'' || c == '$')
			break ;
		i += 1;
	}
	var = arena_alloc(temp.arena, sizeof(char) * i + 1);
	ft_memmove(var, &data->string[index], i);
	var = getenv(var);
	arena_temp_end(&temp);	
	if (var == NULL)
		return (index + i);
	i = 0;
	while (var[i] != '\0')
	{
		filename[index + i] = var[i];
	}
}

char *process_filename(t_arena *arena, t_token *data)
{
	const char *filename;
	char c;
	int i;

	i = 0;
	while (i < data->string_len)
	{
		if (data->string[i] == '"')
			i = match_double_quote(data->string, i + 1, data->string_len);
		if (data->string[i] == '$')
			i = expand_variable(arena, data, i + 1);
		if (data->string[i] == '\'')
			match_single_quote();
	}
}