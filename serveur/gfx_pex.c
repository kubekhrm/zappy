/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gfx_pex.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdefilip <vdefilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/06/24 13:15:44 by vdefilip          #+#    #+#             */
/*   Updated: 2014/06/25 12:29:12 by vdefilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void			pex(t_env *e, int fd, t_bot *bot)
{
	char			buf[BUF_SIZE];

	sprintf(buf, "pex #%d\n", bot->id);
	buf_load(e->fds[fd].buf_write, buf);
}

void			notify_all_gfx_pex(t_env *e, t_bot *bot)
{
	t_iterator		iter;
	t_gfx			*gfx;

	iter = NULL;
	while ((gfx = (t_gfx *)ft_lst_iter_next_content(e->gfx_lst, &iter)))
		pex(e, gfx->fd, bot);
}
