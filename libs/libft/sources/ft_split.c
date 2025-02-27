/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 15:43:46 by ltaalas           #+#    #+#             */
/*   Updated: 2024/12/23 16:45:43 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

static int	count_words(const char *s, char c)
{
	int	count;

	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
			count++;
		while (*s != c && *s)
			s++;
	}
	return (count);
}

static void	free_list(char **arr)
{
	int	i;

	i = 0;
	while (arr[i] != 0)
		free(arr[i++]);
	ft_bzero(arr, i);
	free(arr);
}

static int	split(char **list, char const *s, char c)
{
	int	i;
	int	len;

	i = 0;
	while (*s)
	{
		while (*s == c && *s)
			s++;
		if (*s)
		{
			if (!ft_strchr(s, c))
				len = ft_strlen(s);
			else
				len = (ft_strchr(s, c) - s);
			list[i] = ft_substr(s, 0, len);
			if (list[i++] == NULL)
			{
				free_list(list);
				return (0);
			}
			s += len;
		}
	}
	list[i] = NULL;
	return (42);
}

char	**ft_split(char const *s, char c)
{
	char	**list;

	list = NULL;
	if (!s)
		return (NULL);
	list = (char **)malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (list == NULL)
		return (NULL);
	if (!(split(list, s, c)))
		return (NULL);
	return (list);
}
