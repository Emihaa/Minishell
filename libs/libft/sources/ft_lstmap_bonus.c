/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:25:49 by ltaalas           #+#    #+#             */
/*   Updated: 2024/12/23 16:45:43 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

static int	make_list(t_list *lst, t_list **new,
						void **content_temp, void *(*f)(void *))
{
	while (lst)
	{
		*content_temp = f(lst->content);
		(*new)->next = ft_lstnew(*content_temp);
		if ((*new)->next == NULL)
			return (0);
		lst = lst->next;
		(*new) = (*new)->next;
	}
	return (42);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new;
	t_list	*first;
	void	*content_temp;

	if (lst == NULL || f == NULL || del == NULL)
		return (NULL);
	content_temp = f(lst->content);
	new = ft_lstnew(content_temp);
	if (new == NULL)
	{
		del(content_temp);
		return (NULL);
	}
	first = new;
	lst = lst->next;
	if (!make_list(lst, &new, &content_temp, f))
	{
		del(content_temp);
		ft_lstclear(&first, del);
		return (NULL);
	}
	new->next = NULL;
	return (first);
}
