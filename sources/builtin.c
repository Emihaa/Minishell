/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 17:21:30 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/31 17:24:55 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void builtin_exit(t_minishell *m)
{
	minishell_cleanup(m);
	if (m->pipe_side != -1)
		write(2, "exit\n", 5);
	exit(m->exit_status);
}