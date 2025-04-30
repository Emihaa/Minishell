/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 18:33:23 by ltaalas           #+#    #+#             */
/*   Updated: 2024/12/23 16:45:43 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	int		s1len;
	int		s2len;
	int		strlen;

	str = NULL;
	if (!s1 || !s2)
		return (NULL);
	s1len = ft_strlen(s1);
	s2len = ft_strlen(s2);
	strlen = s1len + s2len + 1;
	str = malloc((sizeof(char) * strlen));
	if (str == NULL)
		return (NULL);
	ft_memmove(str, s1, s1len);
	ft_memmove(&str[s1len], s2, s2len + 1);
	return (str);
}
