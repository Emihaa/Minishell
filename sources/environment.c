/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 18:36:59 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/22 21:45:31 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// int	expand_variable(t_token *data, const uint32_t start, char *ret, char **env)
// {
// 	int i;
// 	uint32_t len;
// 	if (env == NULL)
// 		return (NULL);
// 	ret = NULL;
// 	while (start + len < data->string_len)
// 	{
// 		if (ft_is_white_space(data->string[start + len]) == true)
// 			break ;
// 		if (ft_isalnum(data->string[start + len]) == false)
// 			return (NULL) ;
// 		len += 1;
// 	}
// 	i = 0;
// 	while (env[i] != NULL)
// 	{
// 		if (ft_strncmp(&data->string[start], env[i], len) == 0)
// 		{
// 			ret = env[i];
// 			return (0);
// 		}
// 		i += 1;
// 	}
// 	return (0);
// }

char	*find_env_var(t_token *data, const uint32_t start)
{
	const char **env = get_env(NULL);
	uint32_t i;
	uint32_t len;

	if (env == NULL)
		return (NULL);
	while (start + len < data->string_len)
	{
		if (ft_isalnum(data->u_data.string[start + len]) == false)
			break ;
		len += 1;
	}
	i = 0;
	while (env[i] != NULL)
	{
		if (ft_strncmp(&data->u_data.string[start], env[i], len) == 0)
		{
			return(env[i]);
		}
		i += 1;
	}
	return (NULL);
}
