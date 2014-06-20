/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_request.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfeijoo <cfeijoo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/28 02:26:49 by cfeijoo           #+#    #+#             */
/*   Updated: 2014/06/19 16:37:49 by vdefilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <string.h>
#include "libft.h"
#include "server.h"
#include "requests.h"

t_team			*get_team_by_name(t_env *e, char *name)
{
	t_iterator		iter;
	t_team			*team;

	iter = NULL;
	while ((team = (t_team *)ft_lst_iter_next_content(e->team, &iter)))
	{
		if (ft_strequ(team->name, name))
			return (team);
	}
	return (NULL);
}

void			send_nbr(t_env *e, int fd)
{
	t_bot		*bot;
	char		*nbr;

	bot = get_bot_by_fd(e, fd);
	nbr = ft_itoa(bot->team->unconnected->len + bot->team->queue->len);
	ft_strcat(e->fds[fd].buf_write, nbr);
	ft_strcat(e->fds[fd].buf_write, "\n");
	free(nbr);
}

void			send_dimension(t_env *e, int fd)
{
	char		*width;
	char		*height;

	width = ft_itoa(e->opt.width);
	height = ft_itoa(e->opt.height);
	ft_strcat(e->fds[fd].buf_write, width);
	ft_strcat(e->fds[fd].buf_write, " ");
	ft_strcat(e->fds[fd].buf_write, height);
	ft_strcat(e->fds[fd].buf_write, "\n");
	free(width);
	free(height);
}

void			gfx_connection(t_env *e, int fd)
{
	t_iterator		iter_t;
	t_iterator		iter_b;
	t_team			*t;
	t_bot			*b;

	e->fds[fd].type = FD_GFX_CLIENT;
	ft_lst_pushend(e->gfx_lst, gfx_new(fd));
	printf("Client #%d: connected as GFX\n", fd);
	msz(e, fd);
	sgt(e, fd);
	mct(e, fd);
	tna(e, fd);
	iter_t = NULL;
	while ((t = (t_team *)ft_lst_iter_next_content(e->team, &iter_t)))
	{
		iter_b = NULL;
		while ((b = (t_bot *)ft_lst_iter_next_content(t->connected, &iter_b)))
		{
			pnw(e, fd, b);
			if (b->life_unit <= 0)
				pdi(e, fd, b);
		}
		iter_b = NULL;
		while ((b = (t_bot *)ft_lst_iter_next_content(t->queue, &iter_b)))
		{
			pnw(e, fd, b);
			if (b->life_unit <= 0)
				pdi(e, fd, b);
		}
	}
	//enw...
}

void			bot_connection(t_env *e, int fd, char *team_name)
{
	t_bot		*bot;
	t_team		*team;

	if ((team = get_team_by_name(e, team_name)) == NULL)
		printf("Client #%d: Invalid request (team doesn't exists)\n", fd);
	else if ((bot = connect_bot(e, team)) == NULL)
		printf("Client #%d: Cannot connect to any BOT\n", fd);
	else
	{
		e->fds[fd].type = FD_BOT_CLIENT;
		bot->fd = fd;
		if (bot->sq == -1)
		{
			bot->sq = sq_rand(e);
			move(e, bot, bot->sq);
			gettimeofday(&bot->time, NULL);
			notify_all_gfx_pnw(e, bot);
		}
		printf("Client #%d: connected to BOT #%d\n", fd, bot->id);
		send_nbr(e, fd);
		send_dimension(e, fd);
	}
}

void			bot_parse_request(t_env *e, int fd, char *str)
{
	t_bot		*bot;
	char		**req;

	bot = get_bot_by_fd(e, fd);
	req = (char **)try_void(ft_strsplit(str, ' '), NULL, "malloc");
	if (!req[0] || ((ft_strequ(req[0], "prend") || ft_strequ(req[0], "pose")
		|| ft_strequ(req[0], "broadcast")) && !req[1]))
		printf("Bot client #%d : Invalid request (too few arguments)\n", fd);
	else if (bot->life_unit <= 0)
	{
		printf("Bot client #%d is dead\n", fd);
		ft_strcat(e->fds[fd].buf_write, "mort\n");
	}
	else if (ft_strequ(req[0], "avance"))
		step(e, bot);
	else if (ft_strequ(req[0], "droite"))
		turn_right(e, bot);
	else if (ft_strequ(req[0], "gauche"))
		turn_left(e, bot);
	else if (ft_strequ(req[0], "voir"))
		look(e, bot);
	else if (ft_strequ(req[0], "inventaire"))
		get_inventory(e, bot);
	else if (ft_strequ(req[0], "prend"))
			take(e, bot, req[1]);
	else if (ft_strequ(req[0], "pose"))
			put(e, bot, req[1]);
	else if (ft_strequ(req[0], "inventaire"))
		get_inventory(e, bot);
	else if (ft_strequ(req[0], "expulse"))
		ft_strcat(e->fds[fd].buf_write, "ko\n");
	else if (ft_strequ(req[0], "broadcast"))
		ft_strcat(e->fds[fd].buf_write, "ko\n");
	else if (ft_strequ(req[0], "incantation"))
		ft_strcat(e->fds[fd].buf_write, "ko\n");
	else if (ft_strequ(req[0], "fork"))
		ft_strcat(e->fds[fd].buf_write, "ko\n");
	else if (ft_strequ(req[0], "connect_nbr"))
		send_nbr(e, fd);
	ft_free_strtab(req);
}

void			gfx_parse_request(t_env *e, int fd, char *str)
{
	char		**req;

	req = (char **)try_void(ft_strsplit(str, ' '), NULL, "malloc");
	if (!req[0])
		printf("Gfx client #%d : Invalid request (too few arguments)\n", fd);
	else if (ft_strequ(req[0], "msz"))
		msz(e, fd);
	else if (ft_strequ(req[0], "bct"))
		bct(e, fd, req, 0);
	else if (ft_strequ(req[0], "mct"))
		mct(e, fd);
	else if (ft_strequ(req[0], "tna"))
		tna(e, fd);
	else if (ft_strequ(req[0], "ppo"))
		ppo(e, fd, req, 0);
	else if (ft_strequ(req[0], "plv"))
		plv(e, fd, req, 0);
	else if (ft_strequ(req[0], "pin"))
		pin(e, fd, req, 0);
	else if (ft_strequ(req[0], "sgt"))
		sgt(e, fd);
	else if (ft_strequ(req[0], "sst"))
		sst(e, fd, req);
	ft_free_strtab(req);
}