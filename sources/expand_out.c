/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_out.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:53:44 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/03/28 14:57:02 by ehaanpaa         ###   ########.fr       */
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
static char **expand_stuff(t_arena *arena, t_node *node, char *str)
{
	int i;
	int j;
	char quote;
	// char *env_var;
	static char **argv_pntr;

	j = 0;
	quote = '\0';
	while (j < 2 && arena) // there will only be 2 strings in argv_pntr, no matter what
	{
		i = 0;
		while (i < (int) node->token.string_len) // if string is: "luka" then it should be
		{ // argv_pntr[0] = 'luka' && argv_pntr[1] = '\0'
			if (is_quote(node->token.u_data.string[i]))
			{
				if (quote == '\0' && node->token.u_data.string[i] == '\'')
					quote = '\'';
				else if (quote == '\'' && node->token.u_data.string[i] == '\'')
					quote = '\0';
				else if (node->token.u_data.string[i] == '"')
					i++;
			}
			// if (node->token.u_data.string[i] == '$' && quote != '\'')
			// {
			// 	// first check if '$' naming is correct
			// 	if (ft_isalnum(node->token.u_data.string[i + 1]) == false) //
			// 	{
			// 		printf("alnum is false");
			// 	}
			// 	//move past the $ and start expanding
			// 	env_var = find_env_var(node, ++i, &i); // returns the $PATH so new 'i' is 'i += 4;'
			// 	printf("env_var: %s\n", env_var);
			// }
			
			// if finds white space and it is not '\''
			if (ft_is_white_space(node->token.u_data.string[i]) && quote != '\'')
				break;
			str[i] = node->token.u_data.string[i];
			i++;
		}
		str[i] = '\0';
		argv_pntr[j++] = str;
		printf("redirect argv_pntr[%i]: %s\n", j, str);
	}
	return(argv_pntr);
}


// the nodes should have **argv with 0 = "string" & 1 = NULL, if correct.
// if $PATH expansion gives out more than one word, argv[1] != NULL and will indicate
// 'ambigious redirect' later in code

// if *argv[1] != NULL, argv[0] = "$PATH" <- the string should stay the same as the og expand string
// else if *argv[1] == NULL, argv[0] = "Omppu" <- the string changes to the expanded file name

void expand_out(t_arena *arena, t_node *node)
{
	char **argv_pntr;
	char *str;
	
	printf("--- redirect_out expand ---\n");
	
	argv_pntr = arena_alloc(arena, sizeof(*argv_pntr) * 2);
	str = arena_alloc_no_zero(arena, sizeof(str) * 0);
	argv_pntr = expand_stuff(arena, node, str);
	
	printf("--- out expand done ---\n");	
}
