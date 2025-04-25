/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 01:14:56 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/26 00:37:38 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H

# include "minishell.h"
# include "parser.h"

# define HEREDOC_TEMP_NAME "/tmp/heredoc_temp"
# define NAME_BASE_LEN 17 // sizeof(HEREDOC_TEMP_NAME) - 1
# define EOF_ERROR "minishell: warning: here-document at line %i \
delimited by end-of-file (wanted `%s')\n"

// heredoc_utils.c
uint32_t	set_quote_removed_string(char *dest, t_token *data);
char		*create_temp_file_name(uint32_t heredoc_num);
int			create_heredoc_fds(int fds[2]);
void		print_eof_error(t_minishell *m, char *delimiter);
int			heredoc_read(t_minishell *minishell, char **line, char *delimiter);

// heredoc.c
int			heredoc(t_arena *arena, t_minishell *minishell, t_token *data);

// hooks.c
int			heredoc_event_hook(void);
int			read_loop_event_hook(void);

#endif
