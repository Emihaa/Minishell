/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:00:37 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/03/20 18:15:46 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 

// @TODO: Start working on the syntax errors inside the tree
// there is examples on the Discord chat


// @TODO: REMEMBER TO REMOVE THE GLOBAL VARIABLE USED FOR PRINTING
int value = 1;

// to test:
/*
cc lexer.c tree.c -l readline ../libs/libft/build/libft.a ../libs/lt_alloc/build/lt_alloc.a -o tree
 */


void print_token_type(t_type *token_type);
t_node *find_head_root(t_node *node);


t_node *create_node(t_node *root, t_token *token, t_arena *arena)
{
    t_node *new_node;

    new_node = arena_alloc(arena, sizeof(t_node));
    if (!new_node) //this can be taken away later
    {
        printf("new_node creation failed\n Luka broke something AGAIN\n");
        return(NULL); // probs need to do more than this
    }
    new_node->token = *token;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->root = root;
    if (token->type == PIPE) //this is for printing and can also be removed later
    {
        new_node->token.string = ft_itoa(value++);
        new_node->token.string_len = ft_strlen(new_node->token.string);
    }
    return (new_node);
}
 
// create node in between the root and current node
t_node *insert_middle(t_node *root, t_node *node, t_token *token, t_arena *arena)
{
    t_node *new_node = create_node(root, token, arena);
    if (root)
    {
        if (root->token.type == PIPE && root->left)
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
    // printf(" %.*s\n", (int)node->token->string_len, node->token->string);
    printf("\n");
    return(node);
}

t_node *insert_below(t_node *root, t_node *node, t_token *token, t_arena *arena)
{
    t_node *new_node;
    
    new_node = create_node(root, token, arena);
    node->left = new_node;
    printf("new node--> ");
    print_token_type(&token->type);
    // printf(" %.*s\n", (int)node->token->string_len, node->token->string);
    printf("\n");
    return(new_node);
}

t_node *insert_bottom(t_node *node, t_token *token, t_arena *arena)
{
    t_node *bottom_node = node;
    
    // first we need to find the bottom most node
    while (bottom_node->left)
        bottom_node = bottom_node->left;
    // then create new node to bottom and connect the old bottom with new
    t_node *new_node = create_node(bottom_node, token, arena);
    bottom_node->left = new_node;
    printf("new node--> ");
    print_token_type(&token->type);
    // printf(" %.*s\n", (int)node->token->string_len, node->token->string);
    printf("\n");
    return(new_node);
}

// if token is pipe, we want the pipe to top of everything
// if at the top there is already pipe then we want the new pipe at the
// right side of the old pipe
t_node *insert_top(t_node *node, t_token *token, t_arena *arena)
{
    t_node *head_node = find_head_root(node);
    t_node *new_node;
   // t_node *temp;
    t_node *temp_root;
    
    if (head_node->token.type != PIPE) //at top there isnt any pipes so pipe will be the new top
    {
        new_node = create_node(NULL, token, arena);
        new_node->left = head_node;
        head_node->root = new_node;
    }
    else //there is one or more pipes
    {
        temp_root = head_node;
        //temp = head_node->right;
        while (temp_root->right && temp_root->right->token.type == PIPE) //if multiple pipes, then find the last pipe that
        { // that doesnt have anything on the rigth side
            temp_root = temp_root->right;
        }
        new_node = create_node(temp_root, token, arena);
        new_node->left = temp_root->right;
        temp_root->right = new_node;
        if (new_node->left)
            new_node->left->root = new_node;   
    }
    printf("new node--> ");
    print_token_type(&token->type);
    // printf(" %.*s\n", (int)node->token->string_len, node->token->string);
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

ls > a > b > c | cat | > outfile wc -l | echo smth > 2

                    pipe1
                    /   \
                > a     pipe2
                /       /   \
            > b     cat     pipe3
            /               /   \
        > c         > outfile   > 2
        /               /       /
       ls            wc -l   echo smth
                                
*/

 // Examples above:
 // always adds new nodes to left side on the branch
 // commands/words should be at the bottom
 // redirect should be in the middle
 // pipex always at the top
 
t_node *insert_node(t_node *node, t_node *root, t_token *token, t_arena *arena)
{
    if (!node) // always create new node
    {
        printf("new node--> ");
        print_token_type(&token->type);
        // printf(" %s", token->string);
        printf("\n");
        return (create_node(root, token, arena));
    }
    else if (token->type == WORD) // command or word type
    { // always comes to the bottom of the branch
        printf("   found ");
        print_token_type(&token->type);
        // printf(" %s", token->string);
        if (node->token.type == PIPE) //or the previous token was pipe and we start new brach
        {
            printf(" creating new brach to right if previous node pipe\n");
            printf("new node--> ");
            print_token_type(&token->type);
            // printf(" %s", token->string);
            printf("\n");
            return (node->right = create_node(node, token, arena)); 
        }
        printf(" adding to bottom\n");
        return (insert_bottom(node, token, arena));
    }
    else if (token->type == PIPE) // if it is pipe it always comes to the top, 
    { //or underneath previous pipe
        printf("   found pipe ");
        print_token_type(&token->type);
        printf(" creating node to top\n");
        return (insert_top(node, token, arena)); 
    }
    else // its something different and it comes in the middle of root and current bottom node
    {
        printf("    this is the current node type:");
        print_token_type(&node->token.type);
        // printf(" %s", token->string);
        printf("\n");
        if (node->token.type == PIPE) //or the previous token was pipe and we start new brach
        {
            printf("   found ");
            print_token_type(&token->type);
            printf(" creating new brach to right if previous node pipe\n");
            printf("new node--> ");
            print_token_type(&token->type);
            // printf(" %s", token->string);
            printf("\n");
            return (node->right = create_node(node, token, arena)); 
        }
        else if (node->token.type == WORD)
        {
            printf("   found ");
            print_token_type(&token->type);
            printf(" inserting node to middle\n");
            if (node->root != NULL && node->root->token.type == WORD) //if after right branch, the first node is WORD, all redirects should come first
                return(insert_middle(node->root->root, node->root, token, arena));
            else
                return(insert_middle(node->root, node, token, arena));
        }
        else
        {
            printf("   found ");
            print_token_type(&token->type);
            // printf(" %s", token->string);
            printf(" creating node to under current\n");
            return (insert_below(node, node, token, arena));
        }
    }   
    return (node);
}

// token recognisation function
void print_token_type(t_type *token_type)
{
    if (*token_type == PIPE)
        ft_putstr_fd("|", 2);
    else if (*token_type == REDIRECT_IN)
        ft_putstr_fd("<", 2);
    else if (*token_type == REDIRECT_OUT)
        ft_putstr_fd(">", 2);
    else if (*token_type == HERE_DOCUMENT)
        ft_putstr_fd("<<", 2);
    else if (*token_type == REDIRECT_APPEND)
        ft_putstr_fd(">>", 2);
    else if (*token_type == WORD)
        ft_putstr_fd("WORD", 2);
    else if (*token_type == END_OF_LINE)
        ft_putstr_fd("newline", 2);
    else if (*token_type == ERROR)
        ft_putstr_fd("error", 2);
}

t_node *find_head_root(t_node *node)
{
    if (node && node->root)
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
    printf("[-- ");
    print_token_type(&node->token.type);
    printf(" -> %.*s\n", (int)node->token.string_len, node->token.string);
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
}


// @TODO: fix the tree if only one pipe
// have pointer to previous token that check that
// to get the syntax error for pipe
// if > |
// bash: syntax error near unexpected token `|'
//
// @TODO: create argument vector for nodes
// Luka needs to think about the arena malloc for that
// 
// @TODO: signals in the end
t_node *parser(t_arena *arena, char *line)
{
    t_node *tree;
    t_lexer lexer;
    
    tree = NULL;
    lexer = (t_lexer){.line = line, .line_index = 0};
    while (1)
    {
        t_token token = get_next_token(&lexer);
        if (token.type == END_OF_LINE)
            break ;
        if (token.string_len == 0)
        {
            ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
            t_token token = get_next_token(&lexer);
            print_token_type(&token.type);
            ft_putstr_fd("'\n", 2);
            return (NULL);
        }
        tree = insert_node(tree, NULL, &token, arena);
    }
    tree = find_head_root(tree);
    print_tree(tree, 1);
    expand(arena, tree);
    return (tree);
}


//echo
//for now to test
// int main()
// {
//     int i; 
//     t_node *tree = NULL;
//     t_arena arena = arena_new(DEFAULT_ARENA_CAPACITY);
//     while (1)
// 	{
//         main_loop:
//         tree = NULL;
//         i = 0;
//         value = 1;
//         char *line = readline("minishell >");
//         add_history(line);
//         t_lexer lexer;
//         lexer.line = line;
//         lexer.line_index = 0; 
//         while (1)
//         {
//             t_token token = get_next_token(&lexer);
//             if (token.type == END_OF_LINE)
//                 break ;
//             if (token.string_len == 0)
//             {
//                 ft_putstr_fd("minitalk: syntax error near unexpected token `", 2);
//                 t_token token = get_next_token(&lexer);
//                 print_token_type(&token.type);
//                 ft_putstr_fd("'\n", 2);
//                 goto main_loop;
//             }
//             tree = insert_node(tree, NULL, &token, &arena);
//         }
//         printf("\n");
//         tree = find_head_root(tree);
//         print_tree(tree, 0);
//         free(line);
//         arena_reset(&arena);
//     } 
//     return (0);
// }
