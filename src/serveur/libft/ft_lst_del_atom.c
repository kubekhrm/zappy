/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_del_atom.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdefilip <vdefilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/23 19:08:46 by vdefilip          #+#    #+#             */
/*   Updated: 2014/06/25 19:37:16 by vdefilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

void	ft_lst_del_atom(t_list *lst, t_atom *atom, void (*del)())
{
	if (del != NULL)
		del(atom->content);
	if (atom == lst->curr)
		lst->curr = lst->curr->prev;
	if (atom == lst->first)
		ft_lst_pop(lst);
	else if (atom == lst->last)
		ft_lst_popend(lst);
	else
	{
		atom->prev->next = atom->next;
		atom->next->prev = atom->prev;
		lst->len--;
	}
	free(atom);
}
