/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 19:31:01 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/02 23:01:31 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

inline 
bool is_valid_var_start(char c)
{
	return (ft_isalpha(c) || c == '_');
}

inline
bool	is_quote(char c)
{
	return ((c == '"' || c == '\''));
}

char	*find_env_var(const char *str, const uint32_t str_len, uint32_t *index, char **env)
{
	uint32_t len;
	char c;

	len = 0;
	while (len < str_len)
	{
		c = str[len];
		if (c != '_' && ft_isalnum(c) == false)
			break ;
		len += 1;
	}
	if (env != NULL && len > 0)
	{	
		while (*env != NULL)
		{
			if (ft_strncmp(str, *env, len) == 0 && (*env)[len] == '=')
			{
				*index += len;
				return(&(*env)[len + 1]); // watch out maybe problem
			}
			env++;
		}
	}
	*index += len;
	return (NULL);
}

// builtin exit somebody can add anything they want a exit code
// atol
// exit 100 = $? = 125
// do i write this out or printf it out or what?
int small_itoa(t_expand_vars *v, char *str)
{
	int value;
	int size;

	v->i += 2;
	size = 0;
	value = get_minishell(NULL)->exit_status;
	if (value == 0)
	{
		str[v->len++] = '0'; 
		return (0);
	}
	size = (int) num_len((uint32_t) value);
	str += v->len;
	v->len += size;
	while (value > 0)
	{
		str[--size] = value % 10 + '0';
		value /= 10;
	}	
	return (0);
}
