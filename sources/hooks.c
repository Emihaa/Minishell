/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 00:37:44 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/25 01:22:58 by ehaanpaa         ###   ########.fr       */
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
