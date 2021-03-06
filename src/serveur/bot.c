/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdefilip <vdefilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/06/03 12:23:13 by vdefilip          #+#    #+#             */
/*   Updated: 2014/06/26 17:38:03 by vdefilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

t_bot			*bot_new(t_team *team)
{
	static int		id = 1;
	t_bot			*new;

	new = (t_bot *)try_void(ft_memalloc(sizeof(*new)), NULL, "malloc");
	new->id = id++;
	new->team = team;
	new->fd = -1;
	new->sq = -1;
	new->dir = dir_rand();
	new->status = STATUS_NONE;
	new->life_unit = FOOD_START * FOOD_UNIT;
	new->inventory = ft_lst_new(NULL);
	new->level = 1;
	new->incant.parent = NULL;
	new->parent = NULL;
	gettimeofday(&new->time, NULL);
	new->timer = 0;
	new->food_timer = 0;
	new->action_timer = -1;
	new->buf_action = buf_new();
	return (new);
}

void			bot_iter_all_connected_queued_egg(t_env *e, void (*fct)())
{
	t_iterator		iter_t;
	t_iterator		iter_b;
	t_team			*t;
	t_bot			*b;

	iter_t = NULL;
	while ((t = (t_team *)ft_lst_iter_next_content(e->team, &iter_t)))
	{
		iter_b = NULL;
		while ((b = (t_bot *)ft_lst_iter_next_content(t->connected, &iter_b)))
			fct(e, b);
		iter_b = NULL;
		while ((b = (t_bot *)ft_lst_iter_next_content(t->queue, &iter_b)))
			fct(e, b);
		iter_b = NULL;
		while ((b = (t_bot *)ft_lst_iter_next_content(t->egg, &iter_b)))
			fct(e, b);
	}
}

t_bot			*get_bot_by_fd(t_env *e, int fd)
{
	t_iterator		iter_t;
	t_iterator		iter_b;
	t_team			*t;
	t_bot			*b;

	iter_t = NULL;
	while ((t = (t_team *)ft_lst_iter_next_content(e->team, &iter_t)))
	{
		iter_b = NULL;
		while ((b = (t_bot *)ft_lst_iter_next_content(t->connected, &iter_b)))
		{
			if (b->fd == fd)
				return (b);
		}
	}
	return (NULL);
}

t_bot			*get_bot_by_id(t_env *e, int id)
{
	t_iterator		iter_t;
	t_iterator		iter_b;
	t_team			*t;
	t_bot			*b;

	iter_t = NULL;
	while ((t = (t_team *)ft_lst_iter_next_content(e->team, &iter_t)))
	{
		iter_b = NULL;
		while ((b = (t_bot *)ft_lst_iter_next_content(t->connected, &iter_b)))
		{
			if (b->id == id)
				return (b);
		}
	}
	return (NULL);
}

void			bot_destroy(t_env *e, int fd, char *msg)
{
	t_bot			*bot;

	if ((bot = get_bot_by_fd(e, fd)) != NULL)
		unconnect_bot(e, bot);
	printf("Client #%d", fd);
	if (bot)
		printf(" (BOT #%d)", bot->id);
	printf("gone away");
	if (msg)
		printf(": %s", msg);
	printf("\n");
}
