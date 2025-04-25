/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:06:30 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/25 20:32:16 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _GNU_SOURCE

# include "../libs/lt_arena/includes/lt_arena.h"
# include "../libs/libft/includes/libft.h"

# include <errno.h>				// errno
# include <stddef.h>			// size_t
# include <unistd.h>			// for write, pipe etc.
# include <sys/types.h> 		// pid_t
# include <sys/wait.h>			// wait, waitpid
# include <stdio.h>				// printf
# include <readline/readline.h>	// readline
# include <readline/history.h>	// add_history
# include <linux/limits.h>		// linux numerical limits
# include <stdbool.h>			// for bool data type
# include <fcntl.h>				// open
# include <signal.h>			// signal handling

# define WRITE	1
# define READ	0

/// @brief global integer for signal handling accross heredoc and main
extern volatile sig_atomic_t	g_int;

/// @brief All token token types. 
/// All tokens are negative expect heredoc to catch heredoc
/// @param PIPE				=	-'|'
/// @param REDIRECT_IN		=	-'<'
/// @param REDIRECT_OUT		=	-'>'
/// @param REDIRECT_AMBI	=	-41
/// @param HERE_DOCUMENT	= 	256		<- <<
/// @param REDIRECT_APPEND	=	-257	<- >>
/// @param WORD				=	-260	<- generic word, can be cmd or arg
/// @param END_OF_LINE		=	0		<- 
/// @param ERROR			=	-404	<- used in parser syntax handling
typedef enum e_type
{
	PIPE			=	-'|',
	REDIRECT_IN		=	-'<',
	REDIRECT_OUT	=	-'>',
	REDIRECT_AMBI	=	-41,
	HERE_DOCUMENT	=	256,
	REDIRECT_APPEND	=	-257,
	WORD			=	-260,
	END_OF_LINE		=	0,
	ERROR			=	-404,
}	t_type;

/// @TODO: restucture to reduce memory footprint
/// @brief Globally used parameters 
typedef struct s_minishell
{
	int			istty;
	uint32_t	line_counter;
	uint32_t	command_count;
	uint32_t	heredoc_count;
	int			*heredoc_fds;
	int			redir_fds[2];
	int			pipe[2];
	int8_t		pipe_side;
	pid_t		last_pid;
	int			exit_status;
	char		*line;
	t_arena		*global_arena;
	t_arena		*file_buf;
	char		**envp;
	int			envp_size;
	int			env_capacity;
}	t_minishell;

/// @brief Builtin commands
typedef enum e_builtin
{
	BUILTIN_FALSE = 0,
	BUILTIN_EXIT,
	BUILTIN_ECHO,
	BUILTIN_CD,
	BUILTIN_PWD,
	BUILTIN_ENV,
	BUILTIN_UNSET,
	BUILTIN_EXPORT,
}	t_builtin;

// arena_strings.c
void		string_find_new_memory(t_arena *a, t_string *str, size_t new_size);
void		append_to_string(t_arena *a, t_string *str,
				char *src, size_t src_len);
void		terminate_and_commit_string(t_arena *a, t_string *str);

// arena_utils.c
void		*xarena_alloc(t_arena *arena, uint64_t size);
void		*xarena_alloc_no_zero(t_arena *arena, uint64_t size);
t_arena		*xarena_new(uint64_t cap);
void		*xarena_realloc(t_arena *arena, void *old,
				uint64_t old_size, uint64_t new_size);

// error.c
void		error_exit(t_minishell *m, int exit_status);
void	syscall_failure(t_minishell *m, char *file, int line);

// execute_utils.c
void		close_pipe(t_minishell *m);
void		execve_failure(t_minishell *m, char *cmd);
void		command_not_found(t_minishell *m, char *cmd);

// execute.c
int			execute_command(t_arena *arena, t_minishell *m,
				char **argv, int status);

// minishell_utils.c
void		wait_signal_handler(int signal);
void		minishell_cleanup(t_minishell *minishell);
void		init_minishell(t_minishell *minishell, char **envp);
void		signal_handler(int signal);

// minishell.c
void		close_heredocs(t_minishell *m);
void		wait_for_sub_processes(t_minishell *minishell);
void		minishell_cleanup(t_minishell *minishell);
void		signal_handler(int signal);

// redirect_utils.c
void		store_write_fd(int out_fd, t_minishell *minishell);
void		store_read_fd(int in_fd, t_minishell *minishell);
void		apply_redirect(t_minishell *minishell);
void		reset_redirect(t_minishell *minishell);

// redirect.c
int			redirect_ambi(char *file_name);
int			redirect_out(char *file_name, t_minishell *m);
int			redirect_append(char *file_name, t_minishell *m);
int			redirect_in(char *file_name, t_minishell *m);
// int			redirect_heredoc(t_token *data, t_minishell *m);
// is this used anywhere??

// utils.c
uint32_t	eat_space(char *str);
bool		is_space(char c);
bool		is_quote(char c);
uint8_t		num_len(uint32_t num);
t_minishell	*get_minishell(t_minishell *m);

// write_functions.c
int			write_bytes(int fd, char *str, size_t bytes_to_write);
int			put_str_nl(int fd, char *str);
int			put_str(int fd, char *str);
int			put_char(int fd, char c);

// get_line.c
char		*get_line(t_arena *arena, int fd);

#endif
