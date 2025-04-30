/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:47:28 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/26 17:57:24 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBAL_H
# define GLOBAL_H

# include <signal.h>			// sig_atomic_t type

/// @brief global integer for signal handling accross heredoc and main
extern volatile sig_atomic_t	g_sig;

#endif