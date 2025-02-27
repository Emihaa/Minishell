/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc_fastish.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 18:41:51 by ltaalas           #+#    #+#             */
/*   Updated: 2025/01/09 21:25:54 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>
#include "../includes/fastish.h"

void	*ft_realloc_fastish(void *src, size_t size_src, size_t size_total)
{
	void	*alloc;

	alloc = ft_calloc_fastish(size_total, sizeof(char));
	if (alloc == NULL)
		return (NULL);
	if (size_total < size_src)
		size_src = size_total;
	ft_memmove_fastish(alloc, src, size_src);
	free(src);
	return (alloc);
}
