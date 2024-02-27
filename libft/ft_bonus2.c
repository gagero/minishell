/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggero <ggero@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 23:09:15 by ggero             #+#    #+#             */
/*   Updated: 2023/09/15 19:53:48 by ggero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	new->next = *lst;
	new->prev = NULL;
	*lst = new;
}

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*tmp;

	if (lst == NULL)
		return ;
	if (*lst == NULL)
		*lst = new;
	else
	{
		tmp = ft_lstlast(*lst);
		new->prev = tmp;
		tmp->next = new;
	}
}

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*tmp;

	if (lst == NULL || del == NULL || *lst == NULL)
		return ;
	while ((*lst)->next != NULL)
	{
		tmp = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = tmp;
	}
	ft_lstdelone(*lst, del);
	*lst = NULL;
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*ret;
	t_list	*beg;

	if (lst == NULL)
		return (NULL);
	ret = ft_lstnew(f(lst->content));
	if (ret == NULL)
		return (NULL);
	beg = ret;
	lst = lst->next;
	while (lst != NULL)
	{
		ft_lstadd_back(&ret, ft_lstnew(f(lst->content)));
		if (ret->next == NULL)
		{
			ft_lstclear(&beg, del);
			return (NULL);
		}
		lst = lst->next;
		ret = ret->next;
	}
	ret->next = NULL;
	return (beg);
}

const t_list	*ft_lstindex(const t_list	*lst, const int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		if (lst)
		{
			if (lst->next)
				lst = lst->next;
			else
				return (NULL);
		}
		j++;
	}
	return (lst);
}
