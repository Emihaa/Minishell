/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:53:44 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/19 23:35:25 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


// Change argv->type type AMBIGIOUS_REDI if expand files and
// becomes ambigious redirect. If ambigious redirect then the string is the given argument name.
// Otherwise the string is just what string expands to.
static 
void expand_action(t_arena *arena, t_node *node)
{
	t_string str;
	t_arg arg;
	t_arg left_over;
	int i;
	(void)arena;
	
	i = 0;
	str = (t_string){0};
	arg = (t_arg){0};
	left_over = (t_arg){0};
	init_arg(&node->token, &arg);
	while (arg.i < arg.data_len)
	{
		copy_until_quote_or_var(arena, &arg, &str);
		if (is_quote(arg.data_str[arg.i]))
			handle_quote(arena, &arg, &str);
		else if (arg.data_str[arg.i] == '$')
		{
			if (handle_var(arena, &str, &arg, &left_over)) // then what? if it returns 1 then it is ambigious redirect??
			{
				node->token.type = REDIRECT_AMBI;
				printf("str: %s\n", node->token.string);
				return ;
			}
		}
	}
	append_to_string(arena, &str, node->token.string, i); // <-- is this even necessary if the string doesnt change?
	if (str.size > 0)
		node->token.string = str.base;
	else
	{
		node->token.type = REDIRECT_AMBI;	
		node->token.string = arg.data_str;
	}
	printf("str: %s\n", node->token.string);
}

void expand_redirect(t_arena *arena, t_node *node)
{
	// printf("--- redirect_out expand ---\n");
 	expand_action(arena, node);
	// printf("--- out expand done ---\n");
}
