/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:51:33 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/03/20 19:14:58 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 

// add the string to the first word node and break the connection
static void travel_tree(t_node *node, char *str)
{
    int i;
    t_node *tree_head;
    
    i = 0;
    tree_head = node;
    if (!node)
        return ;

    // travel left subtree
    while (node)
    {
        if (node->token.type == WORD)
        {
            printf("%s\n", node->token.string);
            while (i++ <= (int)node->token.string_len)
            {
                *str++ = *node->token.string++;
            }
            i = 0;
            *str++ = '\0';
        }
        node = node->left;
    }
    travel_tree(tree_head->right, str);
}

// eli siis.
// go through the tree and gather all the words of same branch under same string
// separated by \0
// then create an array of pointers that point to the starting points of the string
// do field splitting
// and remove quates last
void expand(t_arena *arena, t_node *tree)
{
    static char str[500];
    
    if (arena)
        printf("--- at expand ---\n");
    travel_tree(tree, &*str);
    write(1, "str: ", 5);
    write(2, str, 40);
    printf("\n");
}