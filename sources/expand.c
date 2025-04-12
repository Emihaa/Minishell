/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:51:33 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/13 00:33:25 by ltaalas          ###   ########.fr       */
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

typedef struct s_arg
{
	char	*arg;
	char	*data_str;
	uint32_t size;
	uint32_t data_len;
	uint32_t i;
	bool	exist;
} t_arg;

void write_to_arena(t_arena *arena, t_arg *arg, const char *src, uint32_t len)
{
	char *dest;

	if (arg->size + len > arena->capacity - arena->size)
	{
		(void)arena_unalloc(arena, arg->size);
		dest = xarena_alloc(arena, sizeof(*arg->arg) * (arg->size + len));
		(void)ft_memmove(dest, arg->arg, arg->size);
		ft_memmove(&arg->arg[arg->size], src, len);
		arg->size += len;
		arg->arg = dest;
		return ;
	}
	dest = xarena_alloc(arena, sizeof(char) * len);
	ft_memmove(dest, src, len);
	arg->size += len;
}

void	copy_exit_code(t_arena *arena, t_arg *arg, int exit_code)
{
	int	len;
	int i;
	char buff[3];

	len = num_len(exit_code);
	i = len;
	while(i--)
	{
		buff[i] = (exit_code % 10) + '0';
		exit_code /= 10;
	}
	write_to_arena(arena, arg, buff, len);
	arg->i += 2;
}

// maybe dest is not needed
void	copy_until_special_char(t_arena *arena, t_arg *arg)
{
	uint32_t len;

	len = 0;
	while (arg->i + len < arg->data_len)
	{
		if (is_special_char(arg->data_str[arg->i + len]))
			break ;
		len += 1;
	}
	if (len > 0)
		arg->exist = true;
	write_to_arena(arena, arg, &arg->data_str[arg->i], len);
	arg->i += len;
}

void	copy_in_single_quote(t_arena *arena, t_arg *arg)
{
	uint32_t len;

	len = 0;
	arg->i += 1;
	while (arg->i + len < arg->data_len)
	{
		if (arg->data_str[arg->i + len] == '\'')
			break ;
		len += 1;
	}
	write_to_arena(arena, arg, &arg->data_str[arg->i],len);
	arg->i += len + 1;
}
uint32_t get_key_len(char *src, uint32_t src_len)
{
	uint32_t len;

	len = 0;
	while (len < src_len)
	{
		if (src[len] != '_' && ft_isalnum(src[len]) == false)
			break ;
		len += 1;
	}
	return (len);
}

int	get_env_var_index(char *key, uint32_t key_len, t_minishell *m)
{
	uint32_t i;

	i = 0;
	printf("key: %.*s\n", key_len, key);
	if (key_len > 0)
	{
		while(m->envp[i] != NULL)
		{
			if (ft_strncmp(key, m->envp[i], key_len) && m->envp[i][key_len] == '=')
			{

				return (i);
			}
			i += 1;
		}
	}
	return (-1);
}

void	copy_full_env_var(t_arena *arena, t_minishell *m, t_arg *arg)
{
	//const char *env_var = find_env_var(&data->string[start], arg->data_len - arg->i, &start, m->envp);
	uint32_t key_len;
	uint32_t var_len;
	int index;

	key_len = get_key_len(&arg->data_str[arg->i], arg->data_len - arg->i);
	printf("keylen: %i\n", key_len);
	printf("key: %.*s\n", key_len, &arg->data_str[arg->i]);
	index = get_env_var_index(&arg->data_str[arg->i], key_len, m);
	printf("index: %i\n", index);
	if (index != -1)
	{
		printf("var: %s\n", &m->envp[index][key_len]);
		var_len = ft_strlen(&m->envp[index][key_len]);
		write_to_arena(arena, arg, &m->envp[index][key_len], var_len);
	}
	arg->i += key_len;
}

void expand_variable_in_quotes(t_arena *arena, t_minishell *m, t_arg *arg)
{
	if (arg->data_str[arg->i + 1] == '?')
	{
		copy_exit_code(arena, arg, m->exit_status);
		
	}
	else if (is_valid_var_start(arg->data_str[arg->i + 1]))
	{
		arg->i += 1;
		copy_full_env_var(arena, m, arg);
	}
	else
	{
		write_to_arena(arena, arg, &arg->data_str[arg->i], 1);
		arg->i += 1;
	}
}

void	copy_in_double_quote(t_arena *arena, t_minishell *m, t_arg *arg)
{
	uint32_t len;

	arg->i += 1;
	len = 0;
	while (arg->i + len < arg->data_len)
	{
		if (arg->data_str[arg->i + len] == '"')
			break ;
		if (arg->data_str[arg->i + len] == '$')
		{
			write_to_arena(arena, arg, &arg->data_str[arg->i], len);
			arg->i += len;
			expand_variable_in_quotes(arena, m, arg);
			len = 0;
		}
		else
			len += 1;
	}
	write_to_arena(arena, arg, &arg->data_str[arg->i], len);
	arg->i += len;
	if (arg->data_str[arg->i] == '"')
		arg->i += 1;
}

// handle_leftover(t_arena *arena, t_token *prev)
// {

// }
// return status

void	init_arg(t_arena *arena, t_token *data, t_arg *arg)
{
	arg->arg = xarena_alloc(arena, sizeof(*arg->arg) * 0);
	arg->size = 0;
	arg->i = 0;
	arg->data_len = data->string_len;
	arg->data_str = data->string;
	arg->exist = false;

}

char *create_argument(t_arena *arena, t_minishell *m, t_token *data, t_token *prev)
{
	t_arg	arg;

	(void)prev;
	// if (prev != NULL)
	// 	handle_leftover(arena, prev);
	init_arg(arena, data, &arg);
	while (arg.i < data->string_len)
	{
		copy_until_special_char(arena, &arg);
		if (is_quote(data->string[arg.i]))
		{
			if (data->string[arg.i] == '\'')
				copy_in_single_quote(arena, &arg);
			else
				copy_in_double_quote(arena, m, &arg);
			arg.exist = true;
		}
		else if (data->string[arg.i] == '$')
		{
			if (is_valid_var_start(data->string[arg.i + 1]))
			{

				//copy_env_var_and_split()
				arg.i += 1;
				copy_full_env_var(arena, m, &arg);
			}
			else if (data->string[arg.i + 1] ==  '?')
				copy_exit_code(arena, &arg, m->exit_status);
			else
				write_to_arena(arena, &arg, &data->string[arg.i], 1);
			arg.exist = true;
		}
	}
	if (arg.exist == false)
		return (NULL);
	write_to_arena(arena, &arg, "\0", 1); // add thing that moves entire argument to new arena chunk if not enough space
	return (arg.arg);
}


// typedef struct s_string
// {
// 	uint32_t len;
// 	char	 *data;
// } t_string;


char	**create_argv(t_arena *arena, t_minishell *m, t_node *node)
{
	t_arena		temp_arena;
	uint32_t	arg_count;
	char		**arg_vec_temp;
	char		**arg_vec;
	char		*arg;
	//t_token		prev;

	temp_arena = arena_new(sizeof(*arg_vec) * 1024); //will kindof need to be growable
	arg_vec_temp = xarena_alloc(&temp_arena, sizeof(*arg_vec) * 0);
	arg_count = 0;
	while (node)
	{
		arg = create_argument(arena, m, &node->token, NULL);
		if (arg != NULL)
		{
			xarena_alloc(&temp_arena, sizeof(*arg_vec) * 1);
			arg_vec_temp[arg_count] = arg;
			arg_count += 1;
		}
		// if (node->token.string_len > 0)
		// 	continue ; 	// this is just one idea
						// we need some way to make the next argument starting with what is left
						// of the env_var and continuing within the same node
		node = node->left;
	}
	arg_vec = arena_alloc(arena, sizeof(*arg_vec) * (arg_count + 1));
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
				tree->token.argv = create_argv(arena, m, tree);					   // if we encounter weird issues
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
