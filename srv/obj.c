/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdefilip <vdefilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/06/04 19:29:42 by vdefilip          #+#    #+#             */
/*   Updated: 2014/06/17 19:11:35 by vdefilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "libft.h"
#include "server.h"

int			get_obj_type(char *obj_name)
{
	if (ft_strequ(obj_name, FOOD))
		return (OBJ_FOOD);
	if (ft_strequ(obj_name, ROCK1))
		return (OBJ_ROCK1);
	if (ft_strequ(obj_name, ROCK2))
		return (OBJ_ROCK2);
	if (ft_strequ(obj_name, ROCK3))
		return (OBJ_ROCK3);
	if (ft_strequ(obj_name, ROCK4))
		return (OBJ_ROCK4);
	if (ft_strequ(obj_name, ROCK5))
		return (OBJ_ROCK5);
	if (ft_strequ(obj_name, ROCK6))
		return (OBJ_ROCK6);
	return (-1);
}

char		*get_obj_name(int type)
{
	if (type == OBJ_FOOD)
		return (FOOD);
	if (type == OBJ_ROCK1)
		return (ROCK1);
	if (type == OBJ_ROCK2)
		return (ROCK2);
	if (type == OBJ_ROCK3)
		return (ROCK3);
	if (type == OBJ_ROCK4)
		return (ROCK4);
	if (type == OBJ_ROCK5)
		return (ROCK5);
	if (type == OBJ_ROCK6)
		return (ROCK6);
	return (NULL);
}

t_obj		*obj_new(int type)
{
	t_obj		*new;

	new = (t_obj *)try_void(malloc(sizeof(t_obj)), NULL, "malloc");
	new->type = type;
	new->lock = 0;
	return (new);
}

t_obj		*get_obj(t_env *e, int sq, int type)
{
	t_iterator		iter;
	t_obj			*obj;

	iter = NULL;
	while ((obj = (t_obj *)ft_lst_iter_next_content(e->board[sq].obj, &iter)))
	{
		if (obj->type == type)
		{
			ft_lst_del_atom(e->board[sq].obj, iter, NULL);
			return (obj);
		}
	}
	return (NULL);
}

int			take(t_env *e, t_bot *bot, char *obj_name)
{
	t_obj			*obj;
	int				type;

	if ((type = get_obj_type(obj_name)) == -1)
	{
		printf("Bot client #%d : Invalid object\n", bot->fd);
		ft_strcat(e->fds[bot->fd].buf_write, "ko\n");
		return (-1);
	}
	if ((obj = get_obj(e, bot->sq, type)) == NULL)
	{
		printf("Bot client #%d : Object not found\n", bot->fd);
		ft_strcat(e->fds[bot->fd].buf_write, "ko\n");
		return (-1);
	}
	if (type == OBJ_FOOD)
	{
		bot->life_unit += FOOD_UNIT;
		ft_lst_pushend(e->board[sq_rand(e)].obj, obj_new(OBJ_FOOD));
	}
	else
		ft_lst_pushend(bot->inventory, obj);
	bot->action_timer = TAKE_TIME;
	printf("Bot client #%d take object\n", bot->fd);
	ft_strcat(bot->buf_action, "ok\n");
	return (0);
}

int			put(t_env *e, t_bot *bot, char *obj_name)
{
	t_iterator		iter;
	t_obj			*obj;
	int				type;

	if ((type = get_obj_type(obj_name)) == -1)
	{
		printf("Bot client #%d : Invalid object\n", bot->fd);
		ft_strcat(e->fds[bot->fd].buf_write, "ko\n");
		return (-1);
	}
	iter = NULL;
	while ((obj = (t_obj *)ft_lst_iter_next_content(bot->inventory, &iter)))
	{
		if (obj->type == type)
		{
			ft_lst_del_atom(bot->inventory, iter, NULL);
			ft_lst_pushend(e->board[bot->sq].obj, obj);
			bot->action_timer = PUT_TIME;
			printf("Bot client #%d put object\n", bot->fd);
			ft_strcat(bot->buf_action, "ok\n");
			return (0);
		}
	}
	printf("Bot client #%d : Object not found\n", bot->fd);
	ft_strcat(e->fds[bot->fd].buf_write, "ko\n");
	return (-1);
}

void		get_inventory(t_env *e, t_bot *bot)
{
	t_iterator		iter;
	t_obj			*o;
	int				obj[7];
	char			str[128];

	(void)e;
	ft_bzero(obj, sizeof(int) * 7);
	obj[0] = bot->life_unit / FOOD_UNIT;
	iter = NULL;
	while ((o = (t_obj *)ft_lst_iter_next_content(bot->inventory, &iter)))
		obj[o->type]++;
	sprintf(str, "{%s %d, %s %d, %s %d, %s %d, %s %d, %s %d, %s %d}",
	FOOD, obj[0], ROCK1, obj[1], ROCK2, obj[2], ROCK3,
	obj[3], ROCK4, obj[4], ROCK5, obj[5], ROCK6, obj[6]);
	bot->action_timer = INVENTORY_TIME;
	printf("Bot client #%d inventory : %s\n", bot->fd, str);
	ft_strcat(bot->buf_action, str);
}