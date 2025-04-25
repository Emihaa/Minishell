/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 00:37:44 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/25 17:38:15 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/heredoc.h"

int	heredoc_event_hook(void)
{
	if (g_int == SIGINT)
	{
		rl_done = 1;
		return (1);
	}
	return (0);
}

int	read_loop_event_hook(void)
{
	if (g_int == SIGINT)
	{
		get_minishell(NULL)->exit_status = 128 + SIGINT;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_int = 0;
		return (1);
	}
	return (0);
}
