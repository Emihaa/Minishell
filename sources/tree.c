/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:00:37 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/03/11 16:57:55 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 


int value = 1;

// to test:
/*
cc lexer.c tree.c -l readline ../libs/libft/build/libft.a ../libs/lt_alloc/build/lt_alloc.a -o tree
 */


void print_token_type(t_type *token_type);
t_node *find_head_root(t_node *node);

// @TODO: change from token pointer to just pointer
// @TODO: maybe in future give up on array and just call "get_next_token" but for now okei 
t_node *create_node(t_node *root, t_token *token, t_arena *arena)
{
    int i = 0;
    
    t_node *new_node = arena_alloc(arena, sizeof(t_node));
    if (!new_node)
        return(NULL); // probs need to do more than this
    new_node->token = token;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->root = root;
    if (token->type == PIPE)
    {
        new_node->token->string = ft_itoa(value++);
        new_node->token->string_len = ft_strlen(new_node->token->string);
    }
    return (new_node);
}
 
// create node in between the root and current node
// @TODO: what if the previous root node doesnt have root?
t_node *insert_middle(t_node *root, t_node *node, t_token *token, t_arena *arena)
{
    t_node *new_node = create_node(root, token, arena);
    if (root)
    {
        if (root->token->type == PIPE && root->left)
            root->right = new_node;
        else
        {
            root->left = new_node;
            root->right = NULL;
        }
    }
    node->root = new_node;
    if (!new_node->left)
        new_node->left = node;
    else
        new_node->right = node;
    printf("new node--> ");
    print_token_type(&token->type);
    printf(" %.*s\n", (int)node->token->string_len, node->token->string);
    printf("\n");
    return(node);
}


// if token is pipe, we want the pipe to top of everything
// if at the top there is already pipe then we want the new pipe at the
// left side of the old pipe
t_node *insert_top(t_node *root, t_node *node, t_token *token, t_arena *arena)
{
    t_node *head_node = find_head_root(node);
    t_node *new_node;
    t_node *temp;
    t_node *temp_root;
    
    if (head_node->token->type != PIPE) //at top there isnt any pipes so pipe will be the new top
    {
        new_node = create_node(NULL, token, arena);
        new_node->left = head_node;
        head_node->root = new_node;
    }
    else //there is only ONE pipe -- more arent currently included
    {
        temp_root = head_node;
        temp = head_node->right;
        while (temp && temp->token->type == PIPE) //if multiple pipes, then find the last pipe that
        { // that doesnt have anything on the rigth side
            temp_root = temp;
            temp = temp->right;
        }
        //problem is here:
        new_node = create_node(temp_root, token, arena);
        new_node->left = temp_root->right;
        temp_root->right = new_node;
        if (new_node->left)
            new_node->left->root = new_node;   
        // new_node = insert_middle(temp_root, temp, token, arena);
    }
    
    printf("new node--> ");
    print_token_type(&token->type);
    printf(" %.*s\n", (int)node->token->string_len, node->token->string);
    printf("\n");
    
    return (new_node);
}

/* Examples of AST tree:

cat > a > b > c                 |   cat > a > b > c | > outfile
                                |
                                |                  pipe
          > a                   |                /      \
          /                     |             > a     > outfile
       > b                      |             /
       /                        |         > b
    > c                         |         /
    /                           |      > c
  cat                           |      /
                                |    cat

ls > a > b > c | cat | > outfile wc -l | echo smth

                    pipe1
                    /   \
                > a     pipe2
                /       /   \
            > b     cat     pipe3
            /               /   \
        > c         > outfile   echo smth
        /               /
       ls             wc -l
                                
*/

 // what is the logic for tree? how does the branches go
 // Examples above:
 // always adds new nodes to left side on the branch
 // commands/words should be at the bottom
 // redirect should be in the middle
 // pipex always at the top
 

// so problem here. What if i need to insert to right branch and not to left branch?
// I cannot connect the new connection always to left
t_node *insert_node(t_node *node, t_node *root, t_token *token, t_arena *arena)
{
    if (!node) // always create new node
    {
        printf("new node--> ");
        print_token_type(&token->type);
        printf(" %s", token->string);
        printf("\n");
        return (create_node(root, token, arena));
    }
    // else if (token->type == WORD) // command or word type
    // { // always comes to the bottom of the branch
    //     printf("   found ");
    //     print_token_type(&token->type);
    //     printf(" creating right branch\n");
    //     insert_node(node->right, node, token, arena);
    // }
    else if (token->type == PIPE) // if it is pipe it always comes to the top, 
    { //or underneath previous pipe
        printf("   found pipe ");
        print_token_type(&token->type);
        
        printf(" creating node to top\n");
        return (insert_top(root, node, token, arena)); 
    }
    else // its something different and it comes in the middle of root and current bottom node
    {
        printf("    this is the current node type:");
        print_token_type(&node->token->type);
        
        printf(" %s", token->string);
        printf("\n");
        if (node->token->type == PIPE) //or the previous token was pipe and we start new brach
        {
            printf("   found ");
            print_token_type(&token->type);
            printf(" creating node to new brach to right if pipe\n");
            return (node->right = create_node(node, token, arena)); 
        }
        else
        {
            printf("   found ");
            print_token_type(&token->type);
            printf(" creating node to top of current\n");
            return (insert_middle(node->root, node, token, arena));
        }
    }   
    return (node);
}

//token recognisation function?
void print_token_type(t_type *token_type)
{
    printf("token:");
    if (*token_type == PIPE)
        printf(" | ");
    else if (*token_type == REDIRECT_IN)
        printf(" < ");
    else if (*token_type == REDIRECT_OUT)
        printf(" > ");
    else if (*token_type == HERE_DOCUMENT)
        printf(" << ");
    else if (*token_type == REDIRECT_APPEND)
        printf(" >> ");
    else if (*token_type == WORD)
        printf(" WORD ");
    else if (*token_type == END_OF_LINE)
        printf(" \n ");
    else if (*token_type == ERROR)
        printf(" error ");
}

t_node *find_head_root(t_node *node)
{
    if (node->root)
        return (find_head_root(node->root));
    return(node);  
}

// Function to print the tree
static void print_tree(t_node *node, int depth)
{
    int i = 0;
    
    if (!node)
        return ;

    while (i++ < depth)
        printf("  ");
    
    // Print current node
    printf("|-- ");
    print_token_type(&node->token->type);
    printf(" -> %.*s\n", (int)node->token->string_len, node->token->string);
    
    // Print left subtree
    if (node->left)
    {
        printf(" L ");
        print_tree(node->left, depth + 1);
    }

    // Print right subtree
    if (node->right)
    {
        printf(" R ");
        print_tree(node->right, depth + 1);
    }

    // printf("node ");
    // print_token_type(&node->token->type);
    // printf(" ---  %s", node->token->string);
    // printf("\n");
    // t_node *temp = node->left;
    // while (temp)
    // {
    //     printf("  |\n");
    //     printf(" L -");
    //     print_token_type(&temp->token->type);
    //     printf(" ---  %s", temp->token->string);
    //     printf("\n");
    //     temp = temp->left;
    // }
    // temp = node->right;
    // while (temp)
    // {
    //     printf("\n R \n");
    //     printf("  |\n");
    //     if (temp->left)
    //     {
    //         printf("\n  \n");
    //         print_tree(temp, depth + 1);
    //     }
    //     print_token_type(&temp->token->type);
    //     printf(" ---  %s", temp->token->string);
    //     printf("\n");
    //     temp = temp->right;
    // }
}

//for now to test
int main()
{
    int i;
    
    t_node *tree = NULL;
    t_arena arena = arena_new(DEFAULT_ARENA_CAPACITY);
    while (1)
	{
        tree = NULL;
        i = 0;
        value = 1;
        char *line = readline("minishell >");
        add_history(line);
        t_lexer lexer;
        lexer.line = line;
        lexer.line_index = 0; 
        t_token *token_array = get_token_array(&arena, &lexer);
        while (token_array[i].type != END_OF_LINE)
        {
            tree = insert_node(tree, NULL, &token_array[i], &arena);
            i++;
        }
        printf("\n");
        tree = find_head_root(tree);
        print_tree(tree, 0);
        free(line);
        arena_reset(&arena);
    } 
    return (0);
}
