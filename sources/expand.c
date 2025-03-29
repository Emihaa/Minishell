/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:51:33 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/03/29 01:38:03 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 

// jos ekasta expansionista tulee tyhjaa ni se argv skipataan


static char **travel_tree(t_arena *arena, t_node *node, char *str, int count);

// second round around the tree packing the expanded words to one string per branch
// then third time around creating the vector pointers and giving them something to point to
// if all strings are done correctly and they are allocated to the arena correctly
// i dont need to know the amount of pointers before hand because if allocate them dynamically
// they will be next to each other on arena, but cannot allocate anything new in between


// char	*find_env_var(const t_token *data, const uint32_t start, uint32_t *index, char **env)
// {
// 	uint32_t len;
// 	char c;

// 	len = 0;
// 	while (start + len < data->string_len)
// 	{
// 		c = data->u_data.string[start + len];
// 		if (c != '_' && ft_isalnum(c) == false)
// 			break ;
// 		len += 1;
// 	}
// 	if (env != NULL && len > 0)
// 	{	
// 		while (*env != NULL)
// 		{
// 			if (ft_strncmp(&data->u_data.string[start], *env, len) == 0 &&
// 							(*env)[len] == '=')
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
			if (ft_strncmp(str, *env, len) == 0 &&
							(*env)[len] == '=')
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

static
char	**travel_expansion(t_arena *arena, t_node *env_node, char *str, int count)
{
	static uint32_t	i;
	static uint32_t	len;
	static char		**argv_pntr;
	
	i = 0;
	len = 0;
	while (i < env_node->token.string_len)
	{
		if (is_space(env_node->token.u_data.string[i]) == true)
		{
			str[len++] = '\0';
			arena_alloc_no_zero(arena, sizeof(*str) * len);
			while (is_space(env_node->token.u_data.string[i]) == true)
				i++;
			if (env_node->token.u_data.string[i] == '\0')
				break;
			env_node->token.u_data.string += i;
			env_node->token.string_len -= i;
			argv_pntr = travel_expansion(arena, env_node, &str[len], count + 1);
			argv_pntr[count] = str;
			return (argv_pntr);
		}
		str[len++] = env_node->token.u_data.string[i++];
	}
	if (env_node->right->token.string_len > 0)
	{
		arena_alloc_no_zero(arena, len);
		argv_pntr = travel_tree(arena, env_node->right, &str[len], count);
		if (argv_pntr != NULL)
			argv_pntr[count] = str;
		return (argv_pntr);
	}
	if (len > 0)
	{
		str[len++] = '\0';
		arena_alloc_no_zero(arena, sizeof(*str) * len);
	}
	// update the arena with new string len info
	//arena_alloc_no_zero(arena, sizeof(char) * len); //after i know how much info i got i reserve that
	argv_pntr = travel_tree(arena, env_node->left, &str[len], count + (len || 0));
	// when there is no more to go from branch we return
	// and start picking up the pointers   
	// on the way back <-----
	if (argv_pntr != NULL)
		argv_pntr[count] = str;
	return(argv_pntr); //should return the WORD node for ARGV
}

// LUKA\0
// $OMPPU"ASD"="LUKA\0ASD"
// expand to env, new recursion node, with env & rest of node->token.u_data.string + len
// (need to cut off the "OMPPU" from the string as well)
// expansion expands to "LUKA ASD" and field splitting recognices two words so second recursion
// still carrying the old info
// on "ASD" there is no space so no \0, and we will continue to write rest of the node->token.u_data.string +len($OMPPU)
// into the same argument 
// after expansion is done, we will still move forward to node.left.left if not NULL

typedef struct s_expand_vars
{
	uint32_t i;
	uint32_t len;
	char quote;
	char *env_var;
}	t_expand_vars;


static inline
void set_env_var(t_expand_vars *v, t_node *node)
{
	v->env_var = find_env_var	(	
									&node->token.u_data.string[++v->i],
									node->token.string_len,
									&v->i,
									get_minishell(NULL)->envp
								);
}

int expansion_stuffs(t_node *node, t_expand_vars *v, char *str)
{
	if (ft_isalnum(node->token.u_data.string[v->i + 1]) == false)
	{
		if (node->token.u_data.string[v->i + 1] == '?')
		{
			// write m->exit_status; @TODO: <--
			v->i += 1;
			return (0); // stuff????
		}
		str[v->len++] = node->token.u_data.string[v->i++];
		return (0);
	}
	set_env_var(v, node);
	// v->env_var = find_env_var(&node->token, ++v->i, &v->i, get_minishell(NULL)->envp);
	if (v->env_var == NULL)
		return (0);
	if (v->quote == '"')
	{
		while (*v->env_var != '\0')
			str[v->len++] = *v->env_var++;
		return (0) ; // no field split, spaces and tabs are spaces and tabs and dollar dollar
	} 				// else need to do more additional recursions and tabs and spaces are '\0'
	while (is_space(*v->env_var) == true)
	{
		v->env_var++;
	}
	while (is_space(*v->env_var) == false)
	{	
		if (*v->env_var == '\0')
			return (0);
		str[v->len++] = *v->env_var++;
	}
	while (is_space(*v->env_var) == true)
		v->env_var++;
	return (1);
}

/*
	maybe?

	arena_alloc_no_zero(arena, v.len);
	ft_memmove(str, &node->token.u_data.string[v.i], v.len);
	v.i += v.len;

*/

static inline
bool	is_quote(char c)
{
	return ((c == '"' || c == '\''));
}

/*
	if (is_quote(node->token.u_data.string[v.i]) &&
		(v.quote == '\0' || v.quote == node->token.u_data.string[v.i]))
	{
		if (v.quote == '\0')
			v.quote = node->token.u_data.string[v.i];
		else if (v.quote = node->token.u_data.string[v.i])
			v.quote = '\0';
		v.i++;
	}
*/
// @TODO: if we get a token with a string of "" bash treats this as the command '\0' so it creates an empty string and uses that as a command
// add the string to the first word node and break the connection
static char **travel_tree(t_arena *arena, t_node *node, char *str, int count)
{
	static t_expand_vars v;
	static char	 **argv_pntr;

	v.env_var = NULL;
	v.i = 0;
	v.len = 0;
	v.quote = '\0';
	if (node == NULL)
	{
		if (count == 0)
			return (NULL);
		argv_pntr = arena_alloc(arena, sizeof(*argv_pntr) * (count + 1));
		argv_pntr[count] = NULL;
		return (argv_pntr);
	}
	while (v.i < node->token.string_len)
	{
		// if we find quates, flag it? and also what type of quate flag
		if (is_quote(node->token.u_data.string[v.i]) &&
			(v.quote == '\0' || v.quote == node->token.u_data.string[v.i]))
		{
			if (v.quote == '\0')
				v.quote = node->token.u_data.string[v.i];
			else if (v.quote == node->token.u_data.string[v.i])
				v.quote = '\0';
			v.i++;
		}
		else if (node->token.u_data.string[v.i] == '$' && v.quote != '\'')
		{
			if (expansion_stuffs(node, &v, str) == 0)
				continue ;
			if (v.len > 0)
			{
				str[v.len++] = '\0';
				arena_alloc_no_zero(arena, v.len);
			}
			node->token.u_data.string += v.i;
			node->token.string_len -= v.i;
			argv_pntr = travel_expansion(arena,
				&(t_node){
					.left = node->left, .right = node, .root = NULL,
					.token = {
						.type = WORD, .u_data.string = v.env_var, .string_len = ft_strlen(v.env_var)},
						}, &str[v.len], count + (v.len || 0));
			if (argv_pntr != NULL)
				argv_pntr[count] = str;
			return(argv_pntr); //should return the WORD node for ARGV
		}
		else
			str[v.len++] = node->token.u_data.string[v.i++];
	}
	if (v.len > 0)
	{
		str[v.len++] = '\0';
		arena_alloc_no_zero(arena, sizeof(*str) * v.len);
	}
	argv_pntr = travel_tree(arena, node->left, &str[v.len], count + (v.len || 0));
	if (argv_pntr != NULL)
		argv_pntr[count] = str;
	return(argv_pntr); //should return the WORD node for ARGV
}


// eli siis.
// first time i go through the tree, i do expansion of the words
// field splitting and handle the quates
// go through the tree second time and gather all the words of same branch under same string
// separated by \0
// then create an array of pointers that point to the starting points of the string

void expand(t_arena *arena, t_minishell *m, t_node *tree)
{
	char *str;
	t_node *tree_root;
	
	printf("\n---- starting tree expansion ----\n");
	
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
				tree->token.type = heredoc(m, &tree->token);
			// if (tree->token.type == REDIRECT_OUT)
			// 	expand_out(arena, tree);
			if (tree->token.type == WORD)
			{
				str = arena_alloc_no_zero(arena, sizeof(char) * 1); //alloc only the first pointer
				tree->token.u_data.argv = travel_tree(arena, tree, str, 0);
				break ;
			}
			tree = tree->left;
		}
		// printf("test stuff: %i\n", i++);
		// back track to the next branch (if any)
		tree = tree_root->right;
	}
	printf("arena size after expansion: %lu\n", arena->size);
	printf("\n---- tree expanded ----\n\n\n");
}
