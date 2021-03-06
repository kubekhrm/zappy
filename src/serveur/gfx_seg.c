/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gfx_seg.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdefilip <vdefilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/06/24 13:21:08 by vdefilip          #+#    #+#             */
/*   Updated: 2014/06/25 12:27:34 by vdefilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void			seg(t_env *e, int fd, t_team *team)
{
	char			buf[BUF_SIZE];

	sprintf(buf, "seg %s\n", team->name);
	buf_load(e->fds[fd].buf_write, buf);
}

void			notify_all_gfx_seg(t_env *e, t_team *team)
{
	t_iterator		iter;
	t_gfx			*gfx;

	iter = NULL;
	while ((gfx = (t_gfx *)ft_lst_iter_next_content(e->gfx_lst, &iter)))
		seg(e, gfx->fd, team);
}
