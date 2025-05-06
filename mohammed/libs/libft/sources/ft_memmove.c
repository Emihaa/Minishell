/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:52:05 by ltaalas           #+#    #+#             */
/*   Updated: 2024/12/23 16:45:43 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char		*destptr;
	const char	*srcptr;

	destptr = NULL;
	srcptr = NULL;
	if (!dest && !src)
		return (dest);
	destptr = (char *)dest;
	srcptr = (char *)src;
	if (destptr < srcptr)
		while (n-- > 0)
			*destptr++ = *srcptr++;
	else
		while (n--)
			destptr[n] = srcptr[n];
	return (dest);
}
