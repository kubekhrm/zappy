/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gfx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdefilip <vdefilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/06/03 12:28:53 by vdefilip          #+#    #+#             */
/*   Updated: 2014/06/18 16:30:52 by vdefilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "libft.h"
#include "server.h"

t_gfx			*gfx_new(int fd)
{
	t_gfx	*new;

	new = (t_gfx *)try_void(ft_memalloc(sizeof(*new)), NULL, "malloc");
	new->fd = fd;
	return (new);
}

void			gfx_destroy(t_env *e, int fd, char *msg)
{
	t_gfx			*gfx;
	t_iterator		iter;

	printf("Client #%d (GFX) gone away", fd);
	if (msg)
		printf(": %s", msg);
	printf("\n");
	iter = NULL;
	while ((gfx = (t_gfx *)ft_lst_iter_next_content(e->gfx_lst, &iter)))
	{
		if (gfx->fd == fd)
		{
			ft_lst_del_atom(e->gfx_lst, iter, free);
			break ;
		}
	}
	fd_clean(&e->fds[fd]);
	close(fd);
}
