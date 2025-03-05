/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:00:37 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/03/05 00:01:20 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 

// so based on how many tokens I get from lexer i create initialize struct node setup
// that many times?
// the nodes should hold the info of the token type

// Backtracing the tree by first going as far down as possible and coming back up?
// create a root to access all the children?
// root is the newline token
// then the order: from right to left
// pipes tokens
// redirection tokens
// command tokens (at the bottom)

// When executing it returns from left to right
// starting from bottom/command tokens

static void free_tree(t_node *root)
{
    static t_node *temp = NULL;
    
    if (temp)
        free(temp);
    if (root->right)
        free_tree(root->right);
    else if (root->left)
        free_tree(root->left);
    else
    {
        if (!root)
            return ;
        temp = root;
        free_tree(root->root);
        //cant free this too early.   
    }   
}

t_node *create_node(int d, t_node *root)
{
    t_node *new_node = (t_node *)malloc(sizeof(t_node));
    if (!new_node)
        return(NULL); // probs need to do more than this
    new_node->data = d;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->root = root;
    //add token info here?
    return (new_node);
}
 
t_node *insert_node(t_node *node, int d, t_node *root)
{
    static t_node *temp = NULL;
    
    temp = root;
    if (!node) //the first node
        return (create_node(d, root)); 
    // insert new nodes to left
    if (d < node->data)
        node->left = insert_node(node->left, d, node);
    else // and right nodes. So far only creates right nodes
        node->right = insert_node(node->right, d, node);
    return (node);
}


// Function to print the tree
// only prints the right side
static void print_tree(t_node *node, int space)
{   
    if (node == NULL)
        return ;
    printf("this node: %zu\n", node->data);
    if (node->root)
        printf(" root node: %zu\n", node->root->data);
    printf("\n");
    print_tree(node->right, space);
}

//for now to test
int main()
{
    int i = 0;
    t_node *tree = NULL;
    
    while (i < 4)
    {
        tree = insert_node(tree, i, NULL);
        i++;
    }
    print_tree(tree, 2);    
    return (0);
}