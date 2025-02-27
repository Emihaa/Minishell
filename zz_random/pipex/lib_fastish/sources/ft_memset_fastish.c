/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset_fastish.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 19:29:43 by ltaalas           #+#    #+#             */
/*   Updated: 2025/01/09 17:41:39 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdint.h>

static void	*ft_memset_8b(void *dest, uint64_t c, size_t n)
{
	uint64_t	*destptr;

	destptr = NULL;
	if (!dest)
		return (dest);
	destptr = (uint64_t *)dest;
	while (n-- > 0)
		*destptr++ = (uint64_t)c;
	return (dest);
}

static void	*ft_memset_4b(void *dest, uint32_t c, size_t n)
{
	uint32_t	*destptr;

	destptr = NULL;
	if (!dest)
		return (dest);
	destptr = (uint32_t *)dest;
	while (n-- > 0)
		*destptr++ = (uint32_t)c;
	return (dest);
}

static void	*ft_memset_1b(void *dest, uint32_t c, size_t n)
{
	uint8_t	*destptr;

	destptr = NULL;
	if (!dest)
		return (dest);
	destptr = (uint8_t *)dest;
	while (n-- > 0)
		*destptr++ = (uint8_t)c;
	return (dest);
}

// This memset can set 4 byte wide ints instead of the usual 1 byte
// Useful but might cause problems if expected to work like the original memset

void	*ft_memset_fastish(void *dest, uint32_t c, size_t nelem)
{
	void		*r_dest;
	uint64_t	big;

	if ((nelem & (8 - 1)) == 0)
	{
		big = c;
		big = (big << 32 | c);
		r_dest = ft_memset_8b(dest, big, nelem >> 3);
	}
	else if ((nelem & (4 - 1)) == 0)
		r_dest = ft_memset_4b(dest, c, nelem >> 2);
	else
		r_dest = ft_memset_1b(dest, c, nelem);
	return (r_dest);
}
