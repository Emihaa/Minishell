/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove_fastish.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 19:29:43 by ltaalas           #+#    #+#             */
/*   Updated: 2025/01/07 23:13:15 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdint.h>

static void	*ft_memmove_8b(void *dest, const void *src, size_t n)
{
	uint64_t		*destptr;
	const uint64_t	*srcptr;

	destptr = NULL;
	srcptr = NULL;
	if (!dest && !src)
		return (dest);
	destptr = (uint64_t *)dest;
	srcptr = (uint64_t *)src;
	if (destptr < srcptr)
		while (n-- > 0)
			*destptr++ = *srcptr++;
	else
		while (n--)
			destptr[n] = srcptr[n];
	return (dest);
}

static void	*ft_memmove_4b(void *dest, const void *src, size_t n)
{
	uint32_t		*destptr;
	const uint32_t	*srcptr;

	destptr = NULL;
	srcptr = NULL;
	if (!dest && !src)
		return (dest);
	destptr = (uint32_t *)dest;
	srcptr = (uint32_t *)src;
	if (destptr < srcptr)
		while (n-- > 0)
			*destptr++ = *srcptr++;
	else
		while (n--)
			destptr[n] = srcptr[n];
	return (dest);
}

static void	*ft_memmove_1b(void *dest, const void *src, size_t n)
{
	uint8_t			*destptr;
	const uint8_t	*srcptr;

	destptr = NULL;
	srcptr = NULL;
	if (!dest && !src)
		return (dest);
	destptr = (uint8_t *)dest;
	srcptr = (uint8_t *)src;
	if (destptr < srcptr)
		while (n-- > 0)
			*destptr++ = *srcptr++;
	else
		while (n--)
			destptr[n] = srcptr[n];
	return (dest);
}

void	*ft_memmove_fastish(void *dest, const void *src, size_t nelem)
{
	void	*r_dest;

	if ((nelem & (8 - 1)) == 0)
		r_dest = ft_memmove_8b(dest, src, nelem >> 3);
	else if ((nelem & (4 - 1)) == 0)
		r_dest = ft_memmove_4b(dest, src, nelem >> 2);
	else
		r_dest = ft_memmove_1b(dest, src, nelem);
	return (r_dest);
}
