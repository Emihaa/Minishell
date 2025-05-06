/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:55:38 by ltaalas           #+#    #+#             */
/*   Updated: 2024/12/23 16:45:43 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
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
	ft_bzero(arrptr, total);
	return (arrptr);
}
