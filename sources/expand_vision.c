/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vision.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:51:33 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/12 21:30:20 by ltaalas          ###   ########.fr       */
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

void write_to_arena(t_arena *arena, const char *src, uint32_t len)
{
	char *dest;

	dest = xarena_alloc(arena, sizeof(char) * len);
	ft_memmove(dest, src, len);
}
// maybe dest is not needed
uint32_t	copy_until_special_char(t_arena *arena, t_token *data, const uint32_t start)
{
	uint32_t len;

	len = 0;
	while (start + len < data->string_len)
	{
		if (is_special_char(data->string[start + len]))
			break ;
		len += 1;
	}
	write_to_arena(arena, &data->string[start], len);
	return (len);
}

uint32_t	copy_in_single_quote(t_arena *arena, t_token *data, uint32_t start)
{
	uint32_t len;

	len = 0;
	while (start + len < data->string_len)
	{
		if (data->string[start + len] == '\'')
			break ;
		len += 1;
	}
	write_to_arena(arena, &data->string[start], len);
	return (start + len + 1);
}

uint32_t copy_full_env_var(t_arena *arena, t_token *data, uint32_t start)
{
	const char *env_var = find_env_var(&data->string[start], data->string_len - start, &start, get_minishell(NULL)->envp);
	uint32_t len;

	if (env_var == NULL)
		return (0);
	len = ft_strlen(env_var);
	write_to_arena(arena, env_var, len);
	return (start);
}

uint32_t	copy_in_double_quote(t_arena *arena, t_minishell *m, t_token *data, uint32_t index)
{
	uint32_t len;

	len = 0;
	while (index + len < data->string_len)
	{
		if (data->string[index + len] == '"')
			break ;
		if (data->string[index + len] == '$')
		{
			if (data->string[index + len + 1] ==  '?')
				index += copy_exit_code(arena, m->exit_status);
			else if (is_valid_var_start(data->string[index + len]))
			{
				write_to_arena(arena, &data->string[index], len);
				index += len;
				index = copy_full_env_var(arena, data, index);
				len = 0;
			}
			else
				len += 1;
		}
		else
			len += 1;

	}
	write_to_arena(arena, &data->string[index], len);
	return (index + len + 1);
}


int	copy_exit_code(t_arena *arena, int exit_code)
{
	uint8_t	len;
	char	*dest;

	len = num_len(exit_code);
	dest =  xarena_alloc(arena, len);
	while(len-- > 0)
	{
		dest[len] = (exit_code % 10) + '0';
		exit_code /= 10;
	}
	return(2);
}

uint32_t match_single_quote(char *str)
{
	uint32_t len;

	len = 0;
	while(str[len] != '\'' && str[len] != '\0')
	{
		len += 1;
	}
	return (len);
}



// GO TO SLEEP
// THIS IS SHIT
uint32_t count_argument_size(t_minishell *m, t_token *data)
{
	uint32_t i;
	uint32_t len;

	i = 0;
	len = 0;
	while (i < data->string_len)
	{
		while (!is_special_char(data->string[i]))
		{
			i += 1;
			len += 1;
		}
		if (data->string[i] == '\'')
		{
			i += 1;
			len += match_single_quote(data->string[i]);
			i += len;
			if (data->string[i] == '\'')
				i += 1;
		}
		else if (data->string[i] == '"')
		{
			i += 1;
			while (data->string[i] != '"')
			{
				i += 1;
				len += 1;
				if (data->string[i] == '$')
				{
					if (data->string[i + 1] == '?')
					{
						i += 2;
						len += num_len(m->exit_status);
					}
					else if (is_valid_var_start(data->string[i + 1]))
					{
						char *env_var;
						env_var = find_env_var(data->string[++i], data->string_len - i, &i, get_minishell(NULL)->envp);
						if (env_var != NULL)
							len += ft_strlen(env_var);
					}
				}
			}
		}
		else if (data->string[i] == '$')
		{
			if (data->string[i + 1] == '?')
			{
				i += 2;
				len += num_len(m->exit_status);
			}
			else if (is_valid_var_start(data->string[i + 1]))
			{
				char *env_var;
				env_var = find_env_var(data->string[++i], data->string_len - i, &i, get_minishell(NULL)->envp);
				if (env_var != NULL)
				{
					int j = 0;
					while (!is_space(env_var[j]) && env_var[j] != '\0')
					{
						j += 1;
						len += 1;
					}

				}
				
			}
		}
	}
	return (len);
}


// mitas jos sama logiikka ku alkuperasessa expandissa mut rekursio
// tapahtuu vaan per argumentti
// stackoverflow voi silti tapahtua mut vaan jos tulee jotain oooooooo sekoilenkohan


// mitas jos jos jos siis
// jos jos jos jos jos jos jos jos jos
// rekursio tallentaa mista indeksista memmove ja kuinka paljon
// varmaan melkee sama logiikka oikeesti ku siin alkuperasessa
// emma tiia ;_;


// lenght based string string__join thing ??

typedef struct s_string
{
	uint32_t len;
	char	 *data;
} t_string;

t_string	create_argument_segment(t_arena *a, t_minishell *m, t_node *node)
{
	t_string str;

	str.len = 0;
	str.data = node->token.string;
	while (str.len < node->token.string_len)
	{
		while (!is_special_char(str.data[str.len]))
			str.len += 1;
	}
}







char	**create_argv(t_arena *arena, t_minishell *m, t_node *node)
{
	t_arena		temp_arena;
	uint32_t	i;
	uint32_t	arg_count;
	char		**arg_vec_temp;
	char		**arg_vec;
	char		*arg;
	bool		got_argument;
	uint32_t	argument_len;
	char		*current_str;


	temp_arena = arena_new(sizeof(*arg_vec) * 1024); //will kindof need to be growable
	arg_vec_temp = xarena_alloc(&temp_arena, sizeof(*arg_vec) * 0);
	arg_count = 0;
	while (node)
	{
		argument_len = 0;
		current_str = node->token.string;
		while (got_argument = false)
		{
			i = 0;
			while (!is_special_char(current_str[i]))
				i += 1;
			argument_len += 1;
			if (is_quote(current_str[i]))
			{
				if (current_str[i] == '\'')
					i + match_single_quote(&current_str[i]);
			}
			
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
