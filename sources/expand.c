/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:51:33 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/03/22 02:29:40 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 

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


// add the string to the first word node and break the connection
static char **travel_tree(t_arena *arena, t_node *node, char *str, int count)
{
	int i;
	int len;
	char quote;
	t_node *tree_head;
	char	 **str_pntr;
	
	i = 0;
	len = 0;
	tree_head = node;
	quote = '\0';

	// on the way to leaf ------>
	// travel left subtree
	printf("%s\n", node->token.u_data.string);
	str = arena_alloc(arena, sizeof(char) * 1); //alloc only the first pointer
	// what is the order? write stuff to the string if $ then expand
	// keep track of quates but jump over them
	while (i <= node->token.string_len)
	{
		// if we find quates, flag it? and also what type of quate flag
		if ((node->token.u_data.string[i] == '\'') || (node->token.u_data.string[i] == '"'))
		{
			if (quote == '\0')
				quote = node->token.u_data.string[i];
			else if (quote == node->token.u_data.string[i])
				quote = '\0';
			i++;
		}
		// then check if there are $ signs inside the word
		// but also if $ inside ' ' expansion doesnt happen at all
		else if (node->token.u_data.string[i] == '$' && quote != '\'')
		{
			if (quote == '"')
			{
				; // no field split, spaces and tabs are spaces and tabs and dollar dollar
			}
			// else need to do more additional recursions and tabs and spaces are '\0'
			{
				// LUKA\0
				t_node temp = {.left = node->left, .right = NULL, .root = NULL, // .root = node
								.token = {.type = WORD, .u_data.string = env_str,
								.string_len = ft_strlen(env_str)}};
				travel_tree();
			}
			// if you find $ expand it from env
			// what char are acceptable? albanum and _
			// if $? do different stuff
		}
		else
			str[len++] = node->token.u_data.string[i++];
		// then remove the quates
		// take the quates away from the arena alloc?
		// in utils there is set_quote_removed_string, and i would like to use that maybe?
		// i++;
		// len++;
	}
	str[len++] = '\0';
	// update the arena with new string len info
	arena_alloc(arena, sizeof(char) * len); //after i know how much info i got i reserve that
	if (node->left)
		travel_tree(arena, node->left, &str[len], count++);
	else
	{
		str_pntr = arena_alloc(arena, sizeof(char *) * (count + 2));
		str_pntr[count + 1] = NULL;
	}
	// when there is no more to go from branch we return
	// and start picking up the pointers   
	// on the way back <-----
	
	str_pntr[count] = str;
	
	return(str_pntr); //should return the WORD node for ARGV
}


// eli siis.
// first time i go through the tree, i do expansion of the words
// field splitting and handle the quates
// go through the tree second time and gather all the words of same branch under same string
// separated by \0
// then create an array of pointers that point to the starting points of the string
void expand(t_arena *arena, t_node *tree)
{
	// char *str;
	t_node *tree_root;
	
	tree_root = tree;
	printf("---- starting tree expansion ----\n");
	
	//loop here so that we send the first word node of branch
	//come ut of branch, return argv to first word node and then go to another branch loop
	while (1)
	{
		while (tree->token.type != WORD) //find the first WORD node
			tree = tree->left;
		tree->token.u_data.argv = travel_tree(arena, tree, NULL, 0);

		//back track to the next branch (if any)
		if (tree_root->right)
		{
			tree_root = tree_root->right;
			tree = tree_root; 
		}
		else
			break ;
	}
	printf("---- tree expanded ----\n");
}
