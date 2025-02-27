/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc_fastish.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:55:38 by ltaalas           #+#    #+#             */
/*   Updated: 2025/01/07 21:10:56 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>
#include "../includes/fastish.h"

void	*ft_calloc_fastish(size_t nmemb, size_t size)
{
	void	*arrptr;
	size_t	total;

	arrptr = NULL;
	if (nmemb < 1 || size < 1)
		return (malloc(0));
	total = (nmemb * size);
	if (size != total / nmemb)
		return (NULL);
	arrptr = malloc(total);
	if (arrptr == NULL)
		return (NULL);
	ft_memset_fastish(arrptr, '\0', total);
	return (arrptr);
}
