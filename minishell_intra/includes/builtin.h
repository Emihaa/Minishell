/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 23:27:13 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/25 03:15:57 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell.h"

// builtin_utils.c
int			count_argc(char **argv);
int			str_is_numeric(char *str);

// builtin.c
t_builtin	check_for_builtin(char *command);
int			execute_builtin(t_minishell *m, char **argv, t_builtin command);

// cd.c
void		builtin_cd(t_minishell *m, int argc, char **argv);

// env_helpers.c
bool		is_valid_var_start(char c);
uint32_t	get_env_key_len(char *src, uint32_t src_len);
int			get_env_key_index(char *key, uint32_t key_len, char **envp);
char		*get_env_var_value(char *key, uint32_t key_len);

// env.c
int			create_new_env(t_minishell *m, char **envp);
void		builtin_env(char **envp);
char		**create_env(t_minishell *m, char **envp);

// export_print_utils.c
void		print_export_env(char **env);
void		env_sort(char **pntr_array, int size);

// export_utils.c
int			find_slot(t_minishell *m);
int			spelling_check(t_minishell *m, char *s);

// export.c
void		print_export(t_minishell *m);
int			export_add(t_minishell *m, char *s);
void		builtin_export(t_minishell *m, int argc, char **argv);

// unset.c
void		builtin_unset(t_minishell *m, int argc, char **argv);

#endif