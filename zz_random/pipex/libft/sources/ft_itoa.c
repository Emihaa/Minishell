/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:50:52 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/31 23:07:23 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

static int	num_size(int n)
{
	int	i;

	i = 1;
	if (n < 0)
		i++;
	n /= 10;
	while (n)
	{
		n /= 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	int		size;
	char	*num;

	num = NULL;
	size = (num_size(n));
	num = malloc(sizeof(char) * (size + 1));
	if (num == NULL)
		return (NULL);
	if (n == -2147483648)
		return (ft_memmove(num, "-2147483648", 12));
	if (n < 0)
	{
		n *= -1;
		*num = '-';
	}
	num[size--] = '\0';
	while (1)
	{
		num[size--] = n % 10 + '0';
		n /= 10;
		if (n <= 0)
			break ;
	}
	return (num);
}
