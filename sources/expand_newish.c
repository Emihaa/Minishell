/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:51:33 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/09 23:36:16 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 

// jos ekasta expansionista tulee tyhjaa ni se argv skipataan

// LUKA\0
// $OMPPU"ASD"="LUKA\0ASD"
// expand to env, new recursion node, with env & rest of node->token.u_data.string + len
// (need to cut off the "OMPPU" from the string as well)
// expansion expands to "LUKA ASD" and field splitting recognices two words so second recursion
// still carrying the old info
// on "ASD" there is no space so no \0, and we will continue to write rest of the node->token.u_data.string +len($OMPPU)
// into the same argument 
// after expansion is done, we will still move forward to node.left.left if not NULL


static inline
char *set_env_var(t_expand_vars *v, t_node *node)
{
	// This seems problematic
	// node->token.string_len is the total length of the string so seems like index should be deducted from it
	// otherwise there is risk of indexing over the string len;
	v->env_var = find_env_var	(	
									&node->token.string[++v->i],
									node->token.string_len,
									&v->i,
									get_minishell(NULL)->envp
								);
	return (v->env_var);
}

int expansion_stuffs(t_node *node, t_expand_vars *v, char *str)
{
	if (is_valid_var_start(node->token.string[v->i + 1]) == false)
	{
		if (node->token.string[v->i + 1] == '?')
			return (small_itoa(v, str));
		if (is_quote(node->token.string[v->i + 1]))
			v->i += 1;
		else  
			str[v->len++] = node->token.string[v->i++];
		return (0);
	}
	if (set_env_var(v, node) == NULL)
		return (0);
	if (v->quote == '"')
	{
		while (*v->env_var != '\0')
			str[v->len++] = *v->env_var++;
		return (0); // no field split, spaces and tabs are spaces and tabs and dollar dollar
	} 				// else need to do more additional recursions and tabs and spaces are '\0'
	v->env_var += eat_space(v->env_var);
	while (is_space(*v->env_var) == false && *v->env_var != '\0')
		str[v->len++] = *v->env_var++;
	if (*v->env_var == '\0')
		return (0);
	v->env_var += eat_space(v->env_var);
	return (1);
}

int	is_special_char(char c)
{
	return(is_quote(c) || c == '$');
}

void write_to_arena(t_arena *arena, const char *src, uint32_t len, bool *got_argument)
{
	char *dest;

	if (len > 0)
		*got_argument = true;
	dest = xarena_alloc(arena, sizeof(char) * len);
	ft_memmove(dest, src, len);
}
// maybe dest is not needed
uint32_t	copy_until_special_char(t_arena *arena, t_token *data, const uint32_t start, bool *got_argument)
{
	uint32_t len;

	len = 0;
	while (start + len < data->string_len)
	{
		if (is_special_char(data->string[start + len]))
			break ;
		len += 1;
	}
	write_to_arena(arena, &data->string[start], len, got_argument);
	return (len);
}

uint32_t	copy_in_single_quote(t_arena *arena, t_token *data, uint32_t start, bool *got_argument)
{
	uint32_t len;

	len = 0;
	while (start + len < data->string_len)
	{
		if (data->string[start + len] == '\'')
			break ;
		len += 1;
	}
	write_to_arena(arena, &data->string[start], len, got_argument);
	return (start + len + 1);
}

uint32_t copy_full_env_var(t_arena *arena, t_token *data, uint32_t start, bool *got_argument)
{
	const char *env_var = find_env_var(&data->string[start], data->string_len - start, &start, get_minishell(NULL)->envp);
	uint32_t len;

	if (env_var == NULL)
		return (0);
	len = ft_strlen(env_var);
	write_to_arena(arena, env_var, len, got_argument);
	return (start);
}

uint32_t	copy_in_double_quote(t_arena *arena, t_token *data, uint32_t index, bool *got_argument)
{
	uint32_t len;

	len = 0;
	while (index + len < data->string_len)
	{
		if (data->string[index + len] == '"')
			break ;
		if (data->string[index + len] == '$')
		{
			if (is_valid_var_start(data->string[index + len]))
			{
				write_to_arena(arena, &data->string[index], len, got_argument);
				index += len;
				index = copy_full_env_var(arena, data, index, got_argument);
				len = 0;
				continue ;
			}
		}
		len += 1;
	}
	write_to_arena(arena, &data->string[index], len, got_argument);
	return (index + len + 1);
}

char	**create_argv(t_arena *arena, t_node *node)
{
	t_arena		temp_arena;
	uint32_t	i;
	uint32_t	arg_count;
	char		**arg_vec_temp;
	char		**arg_vec;
	char		*arg;
	bool		got_argument;

	temp_arena = arena_new(sizeof(*arg_vec) * 1024); //will kindof need to be growable
	arg_vec_temp = xarena_alloc(&temp_arena, sizeof(*arg_vec) * 0);
	arg_count = 0;
	while (node)
	{
		i = 0;
		got_argument = false;
		arg = xarena_alloc(arena, sizeof(char) * 0);
		while (i < node->token.string_len)
		{
			i = copy_until_special_char(arena, &node->token, i, &got_argument);
			if (is_quote(node->token.string[i]))
			{
				if (node->token.string[i] == '\'')
					i = copy_in_single_quote(arena, &node->token, i + 1, &got_argument);
				else
					i = copy_in_double_quote(arena, &node->token, i + 1, &got_argument);
				got_argument = true;
			}
			else if (node->token.string[i] == '$')
			{
				if (is_valid_var_start(node->token.string[i + 1]))
				{
					//copy_env_var_and_split()
					i = copy_full_env_var(arena, &node->token, i + 1, &got_argument);
					continue ;
				}
				write_to_arena(arena, &node->token.string[i], 1, &got_argument);
			}
		}
		if (got_argument == true)
		{
			xarena_alloc(arena, sizeof(char) * 1);
			xarena_alloc(&temp_arena, sizeof(*arg_vec) * 1);
			arg_vec_temp[arg_count] = arg;
			arg_count += 1;
		}
		node = node->left;
	}

	arg_vec = arena_alloc(arena, sizeof(char *) * (arg_count + 1));
	ft_memmove(arg_vec, arg_vec_temp, sizeof(*arg_vec_temp) * arg_count);
	arena_delete(&temp_arena);
	return (arg_vec);
}




// eli siis.
// first time i go through the tree, i do expansion of the words
// field splitting and handle the quates
// go through the tree second time and gather all the words of same branch under same string
// separated by \0
// then create an array of pointers that point to the starting points of the string

int expand(t_arena *arena, t_minishell *m, t_node *tree)
{
	char *str;
	t_node *tree_root;
	
	// printf("\n---- starting tree expansion ----\n");
	
	//loop here so that we send the first word node of branch
	//come ut of branch, return argv to first word node and then go to another branch loop
	// int i = 0;
	while (tree)
	{
		tree_root = tree;
		//find the first WORD node
		while (tree)
		{
			if (tree->token.type > 0)
			{
				tree->token.type = heredoc(arena, m, &tree->token);
				if (tree->token.type == -2)
					return (-2);
			}
			if (tree->token.type == REDIRECT_OUT || tree->token.type == REDIRECT_IN || tree->token.type == REDIRECT_APPEND)
			 	expand_redirect(arena, tree);
			if (tree->token.type == WORD)
			{
				str = arena_alloc_no_zero(arena, sizeof(char) * 0); //alloc only the first pointer // LUKA: 3.4 took out the 1 byte slack
				tree->token.argv = create_argv(arena, tree);					   // if we encounter weird issues
				tree->left = NULL;																   // it's probably because of this
				break ;
			}
			tree = tree->left;
		} 
		tree = tree_root->right;
	}
	// printf("arena size after expansion: %lu\n", arena->size);
	// printf("\n---- tree expanded ----\n\n\n");
	return (0);
}
