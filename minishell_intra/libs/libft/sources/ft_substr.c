/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 18:09:58 by ltaalas           #+#    #+#             */
/*   Updated: 2024/12/23 16:45:43 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	strlen;

	substr = NULL;
	if (s == NULL)
		return (NULL);
	strlen = ft_strlen(s);
	if (start > strlen)
		start = strlen;
	strlen = ft_strlen(&s[start]);
	if (len > strlen)
		len = strlen;
	substr = malloc((sizeof(char) * (len + 1)));
	if (substr == NULL)
		return (NULL);
	ft_memmove(substr, &s[start], len);
	substr[len] = '\0';
	return (substr);
}
