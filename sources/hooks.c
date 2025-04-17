/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 00:37:44 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/18 00:38:07 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	heredoc_event_hook(void)
{
	if (g_int == SIGINT)
	{
		rl_done = 1;
		return (1);
	}
	return (0);
}

