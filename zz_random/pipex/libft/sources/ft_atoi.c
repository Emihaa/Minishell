/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:55:00 by ltaalas           #+#    #+#             */
/*   Updated: 2025/01/18 00:15:34 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

int	ft_atoi(const char *nptr)
{
	int	i;
	int	num;
	int	sign;

	i = 0;
	num = 0;
	sign = 1;
	while (ft_is_white_space(*nptr))
		nptr++;
	if (*nptr == '-')
	{
		i++;
		sign = -1;
	}
	else if (*nptr == '+')
		i++;
	while (nptr[i] >= '0' && nptr[i] <= '9')
		num = 10 * num + (nptr[i++] - '0');
	return (num * sign);
}
