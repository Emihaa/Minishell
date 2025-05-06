/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 19:52:47 by ehaanpaa          #+#    #+#             */
/*   Updated: 2025/04/27 20:08:14 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/builtin.h"

int	count_argc(char **argv)
{
	int	count;

	if (argv == NULL)
		return (0);
	count = 0;
	while (argv[count] != NULL)
		count++;
	return (count);
}

bool	ft_strtol(const char *nptr, int64_t *dest)
{
	uint64_t	num;
	int			sign;

	num = 0;
	while (ft_is_white_space(*nptr))
		nptr++;
	sign = 0 - (*nptr == '-') + (*nptr != '-');
	if ((*nptr == '-' || *nptr == '+'))
		nptr++;
	if (*nptr < '0' || *nptr > '9')
		return (0);
	while (*nptr >= '0' && *nptr <= '9')
	{
		num = 10 * num + (*nptr++ - '0');
		if ((num > ((uint64_t)INT64_MAX)) && sign == 1)
			return (0);
		else if ((num > ((uint64_t)INT64_MAX + 1)) && sign == -1)
			return (0);
	}
	*dest = (num * sign);
	return (1);
}

int	str_is_numeric(char *str)
{
	int64_t	temp;
	int		i;

	i = 0;
	if (ft_strtol(str, &temp) == 0)
		return (0);
	if (*str == '-' || *str == '+')
		i += 1;
	while (ft_isdigit(str[i]))
		i += 1;
	return ((str[i] == '\0'));
}
