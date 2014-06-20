/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfeijoo <cfeijoo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/24 17:00:40 by vdefilip          #+#    #+#             */
/*   Updated: 2014/06/19 14:31:14 by vdefilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <string.h>
#include "libft.h"
#include "server.h"

static int		check_buf_read(t_env *e, int cs, int type)
{
	int			n;
	char		*p;

	if ((n = recv(cs, e->fds[cs].buf_read, BUF_SIZE, MSG_PEEK)) <= 0)
	{
		if (type == FD_BOT_CLIENT)
			bot_destroy(e, cs, "Connection closed");
		else if (type == FD_GFX_CLIENT)
			gfx_destroy(e, cs, "Connection closed");
		fd_destroy(e, cs, "Connection closed");
	}
	else
	{
		e->fds[cs].buf_read[n] = '\0';
		if ((p = ft_strchr(e->fds[cs].buf_read, '\n')))
		{
			n = p - e->fds[cs].buf_read + 1;
			recv(cs, e->fds[cs].buf_read, n, 0);
			e->fds[cs].buf_read[n - 1] = '\0';
			return (n);
		}
	}
	return (0);
}

void			client_read(t_env *e, int cs)
{
	int		type;

	type =  e->fds[cs].type;
	if (check_buf_read(e, cs, type) > 0)
	{
		if (type == FD_BOT_CLIENT)
		{
			printf("%s", PURPLE);
			printf("Client #%d (BOT) [%s]\n", cs, e->fds[cs].buf_read);
			printf("%s", DEFAULT);
			bot_parse_request(e, cs, e->fds[cs].buf_read);
		}
		else if (type == FD_GFX_CLIENT)
		{
			printf("%s", CYAN);
			printf("Client #%d (GFX) [%s]\n", cs, e->fds[cs].buf_read);
			printf("%s", DEFAULT);
			gfx_parse_request(e, cs, e->fds[cs].buf_read);
		}
		else
		{
			printf("%s", GREEN);
			printf("Client #%d [%s]\n", cs, e->fds[cs].buf_read);
			printf("%s", DEFAULT);
			if (ft_strequ(e->fds[cs].buf_read, "GRAPHIC"))
				gfx_connection(e, cs);
			else
				bot_connection(e, cs, e->fds[cs].buf_read);
		}
		ft_bzero(e->fds[cs].buf_read, BUF_SIZE);
	}
}

void			client_write(t_env *e, int cs)
{
	int		ret;
	int		len;

	len = ft_strlen(e->fds[cs].buf_write);
	if (e->fds[cs].buf_write[len - 1] != '\n')
		ft_strcat(e->fds[cs].buf_write, "\n");
	ret = send(cs, e->fds[cs].buf_write, strlen(e->fds[cs].buf_write), 0);
	if (ret == -1)
	{
		fprintf(stderr, "Cannot send message to Client #%d ", cs);
		if (e->fds[cs].type == FD_BOT_CLIENT)
			fprintf(stderr, "(BOT)\n");
		else
			fprintf(stderr, "(GFX)\n");
	}
	ft_bzero(e->fds[cs].buf_write, BUF_SIZE);
}