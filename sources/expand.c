/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:51:33 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/03/25 00:51:48 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 

static char **travel_tree(t_arena *arena, t_node *node, char *str, int count);

// second round around the tree packing the expanded words to one string per branch
// then third time around creating the vector pointers and giving them something to point to
// if all strings are done correctly and they are allocated to the arena correctly
// i dont need to know the amount of pointers before hand because if allocate them dynamically
// they will be next to each other on arena, but cannot allocate anything new in between


// // add the string to the first word node and break the connection
// static void travel_tree(t_node *node, char *str)
// {
//     int i;
//     int len;
//     t_node *tree_head;  
//     i = 0;
//     len = 0;
//     tree_head = node;
//     if (!node)
//         return ;
//     // travel left subtree
//     while (node)
//     {
//         if (node->token.type == WORD)
//         {
//             //argv pointer spot here
//             printf("%s\n", node->token.u_data.string);
//             while (i <= (int)node->token.string_len)
//             {
//                 str[len++] = node->token.u_data.string[i];
//                 i++;
//             }
//             str[len++] = '\0';
//             i = 0;
//         }
//         node = node->left;
//         //return(len);
//         //argv pointterit 
//     }
//     //new string
//     travel_tree(tree_head->right, str);
// }


// char **get_env(char **env)
// {
// 	static char **envp;
// 	if (env == NULL)
// 		return (envp);
// 	envp = env;
// 	return (NULL);
// }


char	*find_env_var(t_token *data, const uint32_t start, uint32_t *index)
{
	char **env;
	uint32_t len;
	uint32_t i;
	char c;
	
	env = get_minishell(NULL)->envp;
	len = 0;
	while (start + len < data->string_len)
	{
		c = data->u_data.string[start + len];
		if (c == '_' || ft_isalnum(c) == true)
			len += 1;
		else
			break ;
	}
	if (env == NULL || env[0] == NULL || len == 0)
	{	
		*index += len;
		return (NULL);
	}
	i = 0;
	printf("string = %.*s, len = %u\n", len, &data->u_data.string[start], len);
	while (env[i] != NULL)
	{
		if (ft_strncmp(&data->u_data.string[start], env[i], len) == 0)
		{
			
			*index += len;
			return(ft_strchr(env[i], '=') + 1); // watch out maybe problem
		}
		i += 1;
	}
	*index += len;
	return (NULL);
}

static char **travel_expansion(t_arena *arena, t_node *env_node, char *str, int count)
{
	static uint32_t i;
	static uint32_t len;
	char	 **argv_pntr;
	
	i = 0;
	len = 0;

	printf("%s\n", env_node->token.u_data.string);

	while (i < env_node->token.string_len)
	{
		if (is_space(env_node->token.u_data.string[i]) == true)
		{
			str[len++] = '\0';
			arena_alloc_no_zero(arena, sizeof(char) * len);
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
	i = 0;
	while (i < env_node->right->token.string_len)
	{
		if (env_node->right->token.u_data.string[i] == '$')
		{
			arena_alloc_no_zero(arena, len);
			env_node->right->token.u_data.string += i;
			env_node->right->token.string_len -= i;
			argv_pntr = travel_tree(arena, env_node->right, &str[len], count);
			argv_pntr[count] = str;
			return (argv_pntr);
		}
		str[len++] = env_node->right->token.u_data.string[i++];
	}
	str[len++] = '\0';
	arena_alloc_no_zero(arena, len);
	// update the arena with new string len info
	//arena_alloc_no_zero(arena, sizeof(char) * len); //after i know how much info i got i reserve that
	argv_pntr = travel_tree(arena, env_node->left, &str[len], count + 1);
	// when there is no more to go from branch we return
	// and start picking up the pointers   
	// on the way back <-----

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

int quote_stuffs(t_node *node, t_expand_vars *v, char *str)
{
	if (ft_isalnum(node->token.u_data.string[v->i + 1]) == false)
	{
		printf("alnum is false\n");
		if (node->token.u_data.string[v->i + 1] == '?')
		{
			// write m->exit_status;
			v->i += 1;
			return (0); // stuff????
		}
		str[v->len++] = node->token.u_data.string[v->i++];
		return (0);
	}
	printf("node str+index: %u, %s\n", v->i, &node->token.u_data.string[v->i]);
	v->env_var = find_env_var(&node->token, ++v->i, &v->i);
	printf("env_var: %s\n", v->env_var);
	printf("node str+index: %u, %s\n", v->i, &node->token.u_data.string[v->i]);
	if (v->quote == '"')
	{
		while (v->env_var != NULL && *v->env_var != '\0')
		{
			str[v->len++] = *v->env_var++;
		}
		return (0) ; // no field split, spaces and tabs are spaces and tabs and dollar dollar
	}
	// else need to do more additional recursions and tabs and spaces are '\0'
	if (v->env_var == NULL)
		return (0);
	while (*v->env_var != '\0' && is_space(*v->env_var) == false)
	{
		// printf("teeest%u\n", v->len);
		str[v->len++] = *v->env_var++;
	}
	if (*v->env_var == '\0')
		return (0);
	return (1);
}

// add the string to the first word node and break the connection
static char **travel_tree(t_arena *arena, t_node *node, char *str, int count)
{
	static t_expand_vars v;
	char	 **argv_pntr;

	v = (t_expand_vars){0};
	
	if (node == NULL)
	{
		printf("Starting to return!\n");
		argv_pntr = arena_alloc(arena, sizeof(*argv_pntr) * (count + 1));
		argv_pntr[count] = NULL;
		return (argv_pntr);
	}
	// on the way to leaf ------>
	// travel left subtree
	// printf("%s\n", node->token.u_data.string);
	// printf("string_len: %d\n", node->token.string_len);
	
	// what is the order? write stuff to the string if $ then expand
	// keep track of quates but jump over them
	while (v.i < node->token.string_len)
	{
		// if we find quates, flag it? and also what type of quate flag
		if ((node->token.u_data.string[v.i] == '\'') || (node->token.u_data.string[v.i] == '"'))
		{
			if (v.quote == '\0')
				v.quote = node->token.u_data.string[v.i];
			else if (v.quote == node->token.u_data.string[v.i])
				v.quote = '\0';
			else
			{
				str[v.len++] = node->token.u_data.string[v.i++];
				continue ;
			}
			v.i++;
		}
		// then check if there are $ signs inside the word
		// but also if $ inside ' ' expansion doesnt happen at all
		else if (node->token.u_data.string[v.i] == '$' && v.quote != '\'')
		{
			if (quote_stuffs(node, &v, str) == 0)
				continue ;
			str[v.len++] = '\0';
			arena_alloc_no_zero(arena, v.len);
			while (is_space(*v.env_var) == true)
				v.env_var++;
			node->token.u_data.string += v.i;
			node->token.string_len -= v.i;
				// .root = node
			argv_pntr = travel_expansion(arena, 
				&(t_node){
					.left = node->left, .right = node, .root = NULL,
					.token = {
						.type = WORD, .u_data.string = v.env_var, .string_len = ft_strlen(v.env_var)
					},
				},
				&str[v.len],
				count + 1);
			argv_pntr[count] = str;
			return(argv_pntr); //should return the WORD node for ARGV
			// if you find $ expand it from env
			// what char are acceptable? alphanum and '_'
			// if $? do different stuff
		}
		else
		{
			str[v.len++] = node->token.u_data.string[v.i++];
		}
		// then remove the quates
		// take the quates away from the arena alloc?
		// in utils there is set_quote_removed_string, and i would like to use that maybe?
		// i++;
		// len++;
	}
	str[v.len++] = '\0';
	// update the arena with new string len info
	arena_alloc_no_zero(arena, sizeof(*str) * v.len);
	// arena_alloc_no_zero(arena, sizeof(char) * v.len); //after i know how much info i got i reserve that
	argv_pntr = travel_tree(arena, node->left, &str[v.len], count + 1);
	// when there is no more to go from branch we return
	// and start picking up the pointers   
	// on the way back <-----

	// printf("count2: %d\n", count);
	// printf("str: %s\n", str);
	argv_pntr[count] = str;
	
	return(argv_pntr); //should return the WORD node for ARGV
}


// eli siis.
// first time i go through the tree, i do expansion of the words
// field splitting and handle the quates
// go through the tree second time and gather all the words of same branch under same string
// separated by \0
// then create an array of pointers that point to the starting points of the string
void expand(t_arena *arena, t_node *tree)
{
	char *str;
	t_node *tree_root;
	
	tree_root = tree;
	printf("\n---- starting tree expansion ----\n");
	
	//loop here so that we send the first word node of branch
	//come ut of branch, return argv to first word node and then go to another branch loop
	// int i = 0;
	while (1)
	{
		while (tree->token.type != WORD) //find the first WORD node
			tree = tree->left;
		str = arena_alloc_no_zero(arena, sizeof(char) * 1); //alloc only the first pointer
		tree->token.u_data.argv = travel_tree(arena, tree, str, 0);
		// printf("test stuff: %i\n", i++);
		//back track to the next branch (if any)
		if (tree_root->right)
		{
			tree_root = tree_root->right;
			tree = tree_root; 
		}
		else
			break ;
	}
	printf("\n---- tree expanded ----\n\n\n");
}
