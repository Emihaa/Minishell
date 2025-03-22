/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 21:45:48 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/03/19 22:08:34 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../includes/minishell.h"
#include "../libs/libft/includes/libft.h"

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h> // add_history
#include <stdlib.h>

// i could build the envp so that i can work on export
// and then do the expansion using the env and export
// also do unset on env


void export_cmd(int argc, char *argv[], char *envp[])
{
    printf();
    printf("export found\n");
}


int main (int argc, char *argv[], char *envp[])
{
    char *line;
    
    while (1)
    {
        line = readline("export_test> ");
        add_history(line);
        if (ft_strncmp("export", argv[0], 6))
            export_cmd(argc, **argv, **envp);
        free(line);
    }
    return (0);
}