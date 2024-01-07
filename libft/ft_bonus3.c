#include "libft.h"
#include <stdbool.h>
#include <stdlib.h>

t_list	*ft_lstinsert(t_list *lst, t_list *n, const int pos)
{
	t_list	*cont;
	t_list	*cur;

	cur = (t_list *)ft_lstindex(lst, pos);
	if (!cur)
		return (NULL);
	cont = cur->next;
	cur->next = NULL;
	ft_lstadd_back(&cur, n);
	ft_lstlast(cur)->next = cont;
	return (lst);
}

void	ft_lstdelone_reattach(t_list **lst, void (*f)(void *))
{
	f((*lst)->content);
	(*lst)->next->prev = (*lst)->prev;
	(*lst)->prev->next = (*lst)->next;
	free(*lst);
}

t_list	**ft_lstpred(bool (*pred)(bool, t_list *), bool expr, t_list *lst)
{
	t_list	**ret;
	int		i;
	int		j;

	ret = malloc(ft_lstsize(lst) * sizeof(t_list *));
	i = 0;
	while (i < ft_lstsize(lst))
	{
		ret[i] = malloc(sizeof(t_list *));
		i++;
	}
	i = 0;
	j = 0;
	while (i < ft_lstsize(lst))
	{
		if (pred(expr, lst->content) == true)
		{
			ret[j] = lst;
			j++;
		}
		i++;
	}
	return (ret);
}
