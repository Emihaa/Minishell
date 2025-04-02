/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:53:44 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/02 19:19:40 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 

static inline
bool	is_quote(char c)
{
	return ((c == '"' || c == '\''));
}

static void	memmove_name(t_arena *arena, t_node *node, char *str, char **argv)
{
	str = arena_alloc(arena, sizeof(char) * (node->token.string_len + 1));
	ft_memmove(str, node->token.u_data.string, node->token.string_len);
	argv[0] = str;
	argv[1] = str;
}

static void	quote_check(t_node *node, t_expand_vars *v)
{
	if (is_quote(node->token.u_data.string[v->i]))
	{
		if ((v->quote == '\0' && node->token.u_data.string[v->i] == '\'') || \
			(v->quote == '\0' && node->token.u_data.string[v->i] == '\"'))
			v->quote = node->token.u_data.string[v->i];
		else if (v->quote == node->token.u_data.string[v->i])
			v->quote = '\0';
		v->i++;
	}	
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
static void	expand_action(t_arena *arena, t_node *node, char *str, char **argv)
{
	t_expand_vars v;

	v.i = 0;
	v.len = 0;
	v.quote = '\0';
	v.env_var = NULL;
	
	str = arena_alloc_no_zero(arena, sizeof(char) * v.len);
	while (v.i < node->token.string_len)
	{
		quote_check(node, &v);
		if (node->token.u_data.string[v.i] == '$' && v.quote != '\'')
		{
			if (expansion_stuffs(node, &v, str) == 0)
				continue ;
			return (memmove_name(arena, node, str, argv));
		}
		str[v.len++] = node->token.u_data.string[v.i++];
	}
	if (v.len == 0)
		return (memmove_name(arena, node, str, argv));
	str[v.len++] = '\0';
	argv[0] = str;
	str = arena_alloc_no_zero(arena, sizeof(char) * v.len);
	argv[1] = NULL;
}


// the nodes should have **argv with 0 = "string" & 1 = NULL, if correct.
// if $PATH expansion gives out more than one word, argv[1] != NULL and will indicate
// 'ambigious redirect' later in code
// > asd = argv[0] = 'asd' arvgv[1] = 'NULL'
// if *argv[1] != NULL, argv[0] = "$PATH" <- 
// the string should stay the same as the og expand string
// else if *argv[1] == NULL, argv[0] = "Omppu" <- 
// the string changes to the expanded file name
void expand_redirect(t_arena *arena, t_node *node)
{
	char **argv;
	char *str = NULL;
	
	printf("--- redirect_out expand ---\n");
	printf("token string: %.*s\n", node->token.string_len, node->token.u_data.string);
	argv = arena_alloc(arena, sizeof(*argv) * 2);
 	expand_action(arena, node, str, &*argv);
	node->token.u_data.argv = argv;
	printf("redirect argv_pntr[0]: %s, pntr: %p\n", argv[0],  argv[0]);
	printf("redirect argv_pntr[1]: %s, pntr: %p\n", argv[1],  argv[1]);	
	printf("--- out expand done ---\n");	
}
