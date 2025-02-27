/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 17:36:12 by ltaalas           #+#    #+#             */
/*   Updated: 2025/02/23 17:47:09 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <stddef.h>
# include <stdint.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <stdbool.h>
# include <sys/wait.h>
# include <sys/types.h>

# include "libft.h"
# include "fastish.h"

# define READ 0
# define WRITE 1

void		invalid_argument_count(void);
int			is_directory(char *cmd_with_path);
void		close_fds(int *fds, int count);
char		*free_str_arr(char **split);

void		error_command_not_found(char **cmd_argv, char *cmd_with_path,
				char *to_print);
void		error_access(char **cmd_argv, char *cmd_with_path);
void		error_malloc(char **cmd_argv);
void		error_open(int *pipe, char *file);
void		error_execve(char **cmd_argv, char *cmd_with_path);

uint32_t	bin_path_len(const char *bin_paths);
char		*get_bin_paths(char **envp);
char		*check_for_cmd_path(char *cmd_str, char **cmd_argv, char **envp);
char		*get_cmd_with_path(char **cmd_argv, const char *bin_paths);

void		execute_command(char *cmd_str, char **envp);
void		first_child(int pipe[2], char *cmd_str, char *infile, char **envp);
void		last_child(int pipe[2], char *cmd_str, char *outfile, char **envp);

#endif
