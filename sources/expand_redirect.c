/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:53:44 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/25 00:51:18 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/expand.h"

static
char	*memmove_name(t_arena *arena, t_node *node)
{
	char	*str;

	str = xarena_alloc(arena, sizeof(char) * (node->token.string_len + 1));
	ft_memmove(str, node->token.string, node->token.string_len);
	return (str);
}

// Change argv->type to AMBIGIOUS_REDI if expand files and
// becomes ambigious redirect. If ambigious redirect then 
// the string is the given argument name.
// Otherwise the string is just what string expands to.
static
void	expand_action(t_arena *arena, t_node *node, t_string *str, t_arg *arg)
{
	while (arg->i < arg->data_len)
	{
		copy_until_quote_or_var(arena, arg, str);
		if (is_quote(arg->data_str[arg->i]))
			handle_quote(arena, arg, str);
		else if (arg->data_str[arg->i] == '$')
		{
			if (handle_variable(arena, str, arg, NULL))
			{
				arg->exist = false;
				break ;
			}
		}
	}
	if (arg->exist == false)
	{
		node->token.type = REDIRECT_AMBI;
		node->token.string = memmove_name(arena, node);
	}
	else
	{
		terminate_and_commit_string(arena, str);
		node->token.string = str->base;
	}
}

void	expand_redirect(t_arena *arena, t_node *node)
{
	t_string	str;
	t_arg		arg;

	str = (t_string){0};
	arg = (t_arg){0};
	init_arg(&node->token, &arg);
	expand_action(arena, node, &str, &arg);
}
