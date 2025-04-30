/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:35:28 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/30 17:02:46 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "minishell.h"
# include "parser.h"

# define ARGV_DEFAULT_SIZE 32

/// @brief Argument string info, lenght & index.
/// Boolean for argument.exist for NULL terminated argument
typedef struct s_arg
{
	char		*data_str;
	uint32_t	data_len;
	uint32_t	i;
	bool		exist;
}	t_arg;

/// @brief Argument vector that holds the argument data inside it.
/// Also holds the info of memory capacity and currently used size
typedef struct s_arg_vec
{
	size_t	size;
	size_t	capacity;
	char	**data;
}	t_arg_vec;

/// @brief Expand struct
typedef struct s_expand_vars
{
	uint32_t	i;
	uint32_t	len;
	char		quote;
	bool		had_quote;
	char		*env_var;
}	t_expand_vars; // unused

// argv.c
char	**create_argv(t_arena *arena, t_node *node);

// expand_in_helpers.c
void	init_arg(t_token *data, t_arg *arg_vars);
void	init_argv(t_arena *arena, t_arg_vec *argv, size_t cap);
void	realloc_argv(t_arena *arena, t_arg_vec *argv);
int		is_quote_or_var(char c);

// expand_in_quotes.c
void	handle_quote(t_arena *arena, t_arg *arg, t_string *str);

// expand_redirect.c
void	expand_redirect(t_arena *arena, t_node *node);

// expand_utils.c
int		small_itoa(t_expand_vars *v, char *str);
void	copy_exit_code(t_arena *arena, t_arg *arg, t_string *str);

// expand.c
void	copy_until_quote_or_var(t_arena *arena, t_arg *arg, t_string *str);
int		handle_variable(t_arena *arena, t_string *str,
			t_arg *arg, t_arg *leftover);
int		expand(t_arena *arena, t_minishell *m, t_node *tree);

#endif