/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:53:44 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/19 20:40:30 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// // is this used anywhere else???
// char	*find_env_var(const char *str, const uint32_t str_len, uint32_t *index, char **env)
// {
// // WILL NOT WORK PROPERLY IF ENV HAS NULLS BETWEEN StRINGS
// 	uint32_t len;
// 	char c;
// 	len = 0;
// 	while (len < str_len)
// 	{
// 		c = str[len];
// 		if (c != '_' && ft_isalnum(c) == false)
// 			break ;
// 		len += 1;
// 	}
// 	if (env != NULL && len > 0)
// 	{
// 		while (*env != NULL)
// 		{
// 			if (ft_strncmp(str, *env, len) == 0 && (*env)[len] == '=')
// 			{
// 				*index += len;
// 				return(&(*env)[len + 1]); // watch out maybe problem
// 			}
// 			env++;
// 		}
// 	}
// 	*index += len;
// 	return (NULL);
// }

// // @TODO fix expand redirect. Get ridd of the set_env_var and expansion_stuffs
// // rewrite the stuff using the new expand functions

// static inline
// char *set_env_var(t_expand_vars *v, t_node *node)
// {
// 	// This seems problematic
// 	// node->token.string_len is the total length of the string so seems like index should be deducted from it
// 	// otherwise there is risk of indexing over the string len;
// 	v->i += 1;
// 	v->env_var = find_env_var	(
// 									&node->token.string[v->i],
// 									node->token.string_len,
// 									&v->i,
// 									get_minishell(NULL)->envp
// 								);
// 	return (v->env_var);
// }

// int expansion_stuffs(t_node *node, t_expand_vars *v, char *str)
// {
// 	if (is_valid_var_start(node->token.string[v->i + 1]) == false)
// 	{
// 		if (node->token.string[v->i + 1] == '?')
// 			return (small_itoa(v, str));
// 		if (is_quote(node->token.string[v->i + 1]))
// 			v->i += 1;
// 		else
// 			str[v->len++] = node->token.string[v->i++];
// 		return (0);
// 	}
// 	if (set_env_var(v, node) == NULL)
// 		return (0);
// 	if (v->quote == '"')
// 	{
// 		while (*v->env_var != '\0')
// 			str[v->len++] = *v->env_var++;
// 		return (0); // no field split, spaces and tabs are spaces and tabs and dollar dollar
// 	} 				// else need to do more additional recursions and tabs and spaces are '\0'
// 	v->env_var += eat_space(v->env_var);
// 	while (is_space(*v->env_var) == false && *v->env_var != '\0')
// 		str[v->len++] = *v->env_var++;
// 	if (*v->env_var == '\0')
// 		return (0);
// 	v->env_var += eat_space(v->env_var);
// 	return (1);
// }


// static void	memmove_name(t_arena *arena, t_node *node, char *str, char **argv)
// {
// 	str = arena_alloc(arena, sizeof(char) * (node->token.string_len + 1));
// 	ft_memmove(str, node->token.string, node->token.string_len);
// 	argv[0] = str;
// 	argv[1] = str;
// }

// int	quote_check(t_node *node, t_expand_vars *v)
// {
// 	if (is_quote(node->token.string[v->i]))
// 	{
// 		if (v->quote == '\0' && node->token.string[v->i] == '\'')
// 		{
// 			v->quote = node->token.string[v->i];
// 		}
// 		if (v->quote == '\0' && node->token.string[v->i] == '\"')
// 		{
// 			v->quote = node->token.string[v->i];
// 		}
// 		else if (v->quote == node->token.string[v->i])
// 			v->quote = '\0';
// 		v->i++;
// 		v->had_quote = 1;
// 		return (1);
// 	}
// 	return (0);
// }

// // we have to handle the "'" as well
// // if "\"" "$OMPPU" = 'Lukan kissa'
// // no field split, spaces and tabs are spaces and tabs and dollar dollar
// // just make the basics of the function first work
// // dont think of the expansion just yet.
// // return the argv[0] = 'luka'
// // and the argv[1] = "\0"
// // if it is 'luka on'
// // then return argv[0] = 'luka'
// // and argv[1] = 'on'
// // lets get started with basics first then think about the expand
// static void	expand_action(t_arena *arena, t_node *node, char *str, char **argv)
// {
// 	t_expand_vars v;
// 	v.i = 0;
// 	v.len = 0;
// 	v.quote = '\0';
// 	v.env_var = NULL;

// 	str = arena_alloc_no_zero(arena, sizeof(char) * v.len);
// 	while (v.i < node->token.string_len)
// 	{
// 		if (quote_check(node, &v))
// 			continue ;
// 		if (node->token.string[v.i] == '$' && v.quote != '\'')
// 		{
// 			if (expansion_stuffs(node, &v, str) == 0)
// 				continue ;
// 			return (memmove_name(arena, node, str, argv));
// 		}
// 		str[v.len++] = node->token.string[v.i++];
// 	}
// 	if (v.len == 0)
// 		return (memmove_name(arena, node, str, argv));
// 	str[v.len++] = '\0';
// 	argv[0] = str;
// 	str = arena_alloc_no_zero(arena, sizeof(char) * v.len);
// 	argv[1] = NULL;
// }


static void expand_action(t_arena *arena, t_node *s_node)
{
	
	
}

// Change argv->type type AMBIGIOUS_REDI if expand files and
// becomes ambigious redirect. If ambigious redirect then the string is the given argument name.
// Otherwise the string is just what string expands to.
void expand_redirect(t_arena *arena, t_node *node)
{
	// printf("--- redirect_out expand ---\n");
	// printf("token string: %.*s\n", node->token.string_len, node->token.u_data.string);
 	expand_action(arena, node);
	// node->token.argv = argv;
	// printf("redirect argv_pntr[0]: %s, pntr: %p\n", argv[0],  argv[0]);
	// printf("redirect argv_pntr[1]: %s, pntr: %p\n", argv[1],  argv[1]);
	// printf("--- out expand done ---\n");
}
