/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opus1io <opus1io@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/18 13:16:54 by opus1io           #+#    #+#             */
/*   Updated: 2019/01/18 13:37:49 by opus1io          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nm.h>

static void	ft_split_list(t_list* list, t_list** front, t_list** back)
{
	t_list*	fast;
	t_list*	slow;

	slow = list;
	fast = list->next;
	while (fast != NULL)
	{
		fast = fast->next;
		if (fast != NULL)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}

	*front = list;
	*back = slow->next;
	slow->next = NULL;
}

static t_list	*ft_sorted_merge(t_list* a, t_list* b)
{
	t_list		*result;
	t_syminfo	*content_a;
	t_syminfo	*content_b;

	result = NULL;

	if (a == NULL)
		return(b);
	else if (b == NULL)
		return(a);

	content_a = a->content;
	content_b = b->content;
	if (ft_strcmp(content_a->str, content_b->str) < 0)
	{
		result = a;
		result->next = ft_sorted_merge(a->next, b);
	}
	else
	{
		result = b;
		result->next = ft_sorted_merge(a, b->next);
	}
	return(result);
}

void			ft_sort_alpha(t_list **list)
{
	t_list*	head;
	t_list*	a;
	t_list*	b;

	a = NULL;
	b = NULL;
	head = *list;
	if ((head == NULL) || (head->next == NULL))
		return;
	ft_split_list(head, &a, &b);

	ft_sort_alpha(&a);
	ft_sort_alpha(&b);

	*list = ft_sorted_merge(a, b);
}

void			ft_reverse_list(t_list **list)
{
	t_list	*curr;
	t_list	*prev;
	t_list	*next;

	curr = *list;
	prev = NULL;
	next = NULL;
	while (curr)
	{
		next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}
	*list = prev;
}
