/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 19:31:01 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/02 19:53:54 by ltaalas          ###   ########.fr       */
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
