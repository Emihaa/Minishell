/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:53:44 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/01 18:56:01 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 

static inline
bool	is_quote(char c)
{
	return ((c == '"' || c == '\''));
}

// we have to handle the "'" as well
// if "\"" "$OMPPU" = 'Lukan kissa'
// no field split, spaces and tabs are spaces and tabs and dollar dollar

// just make the basics of the function first work
// dont think of the expansion just yet.
// return the argv[0] = 'luka'
// and the argv[1] = "\0"
// if it is 'luka on'
// then return argv[0] = 'luka'
// and argv[1] = 'on'
// lets get started with basics first then think about the expand
static void expand_stuff(t_arena *arena, t_node *node, char *str, char **argv_pntr)
{
	t_expand_vars v;

	v.i = 0;
	v.len = 0;
	v.quote = '\0';
	v.env_var = NULL;
	
	str = arena_alloc_no_zero(arena, sizeof(char) * v.len);
	while (v.i < node->token.string_len)
	{
		if (is_quote(node->token.u_data.string[v.i]))
		{			if ((v.quote == '\0' && node->token.u_data.string[v.i] == '\'') || (v.quote == '\0' && node->token.u_data.string[v.i] == '\"' ))
				v.quote = node->token.u_data.string[v.i];
			else if (v.quote == node->token.u_data.string[v.i])
				v.quote = '\0';
			v.i++;
		}
		if (node->token.u_data.string[v.i] == '$' && v.quote != '\'')
		{
			// if 0 then continue and 1 if something was wrong
			if (expansion_stuffs(node, &v, str) == 0)
				continue ;
			printf("env_var: %s\n", v.env_var);
			
			str = arena_alloc(arena, sizeof(char) * (node->token.string_len + 1));
			ft_memmove(str, node->token.u_data.string, node->token.string_len);
			argv_pntr[0] = str;
			argv_pntr[1] = str;
			return ;
			// ft_memmo
			// something is wrong, argv[0] == og string;
			// alloc token.u_data.string len amount to memore
			// memmove the string to argv[0] = str and argv[1] give same pointer so it isnt NULL
		}
		str[v.len] = node->token.u_data.string[v.i];
		v.i++;
		v.len++;
	}
	if (v.len == 0)
	{
		str = arena_alloc(arena, sizeof(char) * (node->token.string_len + 1));
		ft_memmove(str, node->token.u_data.string, node->token.string_len);
		argv_pntr[0] = str;
		argv_pntr[1] = str;
		return ;
	}
	str[v.len++] = '\0';
	argv_pntr[0] = str;
	str = arena_alloc_no_zero(arena, sizeof(char) * v.len);
	argv_pntr[1] = NULL;
}

// the nodes should have **argv with 0 = "string" & 1 = NULL, if correct.
// if $PATH expansion gives out more than one word, argv[1] != NULL and will indicate
// 'ambigious redirect' later in code

// > asd = argv[0] = 'asd' arvgv[1] = 'NULL'
// if *argv[1] != NULL, argv[0] = "$PATH" <- the string should stay the same as the og expand string
// else if *argv[1] == NULL, argv[0] = "Omppu" <- the string changes to the expanded file name

void expand_redirect(t_arena *arena, t_node *node)
{
	char **argv_pntr;
	char *str = NULL;
	
	printf("--- redirect_out expand ---\n");
	printf("token string: %.*s\n", node->token.string_len, node->token.u_data.string);
	argv_pntr = arena_alloc(arena, sizeof(*argv_pntr) * 2);
	// str = arena_alloc_no_zero(arena, sizeof(str) * 0);
 	expand_stuff(arena, node, str, &*argv_pntr);
	node->token.u_data.argv = argv_pntr;
	printf("redirect argv_pntr[0]: %s, pntr: %p\n", argv_pntr[0],  argv_pntr[0]);
	printf("redirect argv_pntr[1]: %s, pntr: %p\n", argv_pntr[1],  argv_pntr[1]);
	
	printf("--- out expand done ---\n");	
}
