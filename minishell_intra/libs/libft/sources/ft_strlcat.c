/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 15:59:09 by ltaalas           #+#    #+#             */
/*   Updated: 2024/12/23 16:45:43 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	char		*dstptr;
	const char	*srcptr;
	size_t		dstlen;
	size_t		i;

	dstptr = dst;
	srcptr = src;
	i = size;
	while (i-- && *dstptr)
		dstptr++;
	dstlen = dstptr - dst;
	i = size - dstlen;
	if (i == 0)
		return (dstlen + ft_strlen(srcptr));
	while (*srcptr)
	{
		if (i > 1)
		{
			*dstptr++ = *srcptr;
			i--;
		}
		srcptr++;
	}
	*dstptr = '\0';
	return (dstlen + (srcptr - src));
}
