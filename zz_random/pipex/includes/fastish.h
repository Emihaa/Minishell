/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fastish.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 21:00:48 by ltaalas           #+#    #+#             */
/*   Updated: 2025/01/07 21:08:41 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FASTISH_H
# define FASTISH_H

# include <unistd.h>
# include <stdint.h>
# include <stddef.h>

void	*ft_calloc_fastish(size_t nmemb, size_t size);
void	*ft_realloc_fastish(void *src, size_t size_src, size_t size_total);
void	*ft_memmove_fastish(void *dest, const void *src, size_t nelem);
void	*ft_memset_fastish(void *dest, uint32_t c, size_t nelem);

#endif
