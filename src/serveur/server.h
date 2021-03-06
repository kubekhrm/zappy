/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdefilip <vdefilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/06/25 13:04:04 by vdefilip          #+#    #+#             */
/*   Updated: 2014/06/26 17:55:49 by vdefilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <sys/socket.h>
# include <sys/resource.h>
# include <netdb.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/select.h>
# include <sys/time.h>
# include "libft.h"

# define CONNEXION_QUEUE  42
# define CONNEXION_LIMIT  400

# define FD_FREE          0
# define FD_SRV           1
# define FD_BOT_SRV       2
# define FD_GFX_SRV       3
# define FD_CLIENT        4
# define FD_BOT_CLIENT    5
# define FD_GFX_CLIENT    6

# define DEFAULT_BOT_PORT 1984
# define DEFAULT_GFX_PORT 1984
# define DEFAULT_W        30
# define DEFAULT_H        20
# define DEFAULT_LIMIT    10
# define DEFAULT_T        100

# define MIN_W      10
# define MIN_H      10
# define MAX_W      100
# define MAX_H      100
# define MAX_LIMIT  20
# define MAX_T      100

# define BUF_SIZE   1024

# define DEFAULT   "\033[0m"
# define HIGHLIGHT "\033[1m"
# define UNDERLINE "\033[4m"
# define BLINK     "\033[5m"

# define BLACK     "\033[30m"
# define RED       "\033[31m"
# define GREEN     "\033[32m"
# define YELLOW    "\033[33m"
# define BLUE      "\033[34m"
# define PURPLE    "\033[35m"
# define CYAN      "\033[36m"
# define WHITE     "\033[37m"

# define X 0
# define Y 1

# define MAX(a, b) (a > b ? a : b);
# define MIN(a, b) (a < b ? a : b);
# define ABS(a) (a < 0 ? -a : a)

# define OBJ_NB    7
# define OBJ_FOOD  0
# define OBJ_ROCK1 1
# define OBJ_ROCK2 2
# define OBJ_ROCK3 3
# define OBJ_ROCK4 4
# define OBJ_ROCK5 5
# define OBJ_ROCK6 6

# define FOOD  "nourriture"
# define ROCK1 "linemate"
# define ROCK2 "deraumere"
# define ROCK3 "sibur"
# define ROCK4 "mendiane"
# define ROCK5 "phiras"
# define ROCK6 "thystame"

# define NORTH 1
# define EAST  2
# define SOUTH 3
# define WEST  4

# define FOOD_UNIT 126
# define FOOD_START 10

# define STEP_TIME        7
# define TURN_RIGHT_TIME  7
# define TURN_LEFT_TIME   7
# define LOOK_TIME        7
# define INVENTORY_TIME   1
# define TAKE_TIME        7
# define PUT_TIME         7
# define EXPULSE_TIME     7
# define BROADCAST_TIME   7
# define INCANTATION_TIME 300
# define FORK_TIME        42
# define EGG_TIME         600

# define STATUS_NONE        0
# define STATUS_FORK        1
# define STATUS_EGG         2
# define STATUS_INCANTATION 3

# define OBJ_UNLOCKED 0
# define OBJ_LOCKED   1

typedef struct rlimit	t_rlimit;
typedef struct timeval	t_tv;
typedef unsigned long	t_ulong;
typedef struct s_bot	t_bot;

typedef struct	s_buf
{
	t_list		*list;
}				t_buf;

typedef struct	s_parse
{
	char		*cmd;
	void		(*fct)();
}				t_parse;

typedef struct	s_opt
{
	char		*prog_name;
	int			bot_port;
	int			gfx_port;
	int			width;
	int			height;
	t_list		*team_name;
	int			limit;
	int			t;
}				t_opt;

typedef struct	s_gfx
{
	int			id;
	int			fd;
}				t_gfx;

typedef struct	s_fd
{
	int			type;
	void		(*fct_read)();
	void		(*fct_write)();
	t_buf		*buf_read;
	t_buf		*buf_write;
	char		*addr;
	int			port;
}				t_fd;

typedef struct	s_team
{
	char		*name;
	int			limit;
	t_list		*connected;
	t_list		*unconnected;
	t_list		*queue;
	t_list		*egg;
}				t_team;

typedef struct	s_incant
{
	t_bot		*parent;
	t_list		*req[7];
}				t_incant;

struct			s_bot
{
	int			id;
	t_team		*team;
	int			fd;
	int			sq;
	int			dir;
	int			status;
	int			life_unit;
	t_list		*inventory;
	int			level;
	t_bot		*parent;
	t_incant	incant;
	t_tv		time;
	long		timer;
	long		food_timer;
	long		action_timer;
	t_buf		*buf_action;
};

typedef struct	s_obj
{
	int			type;
	int			lock;
}				t_obj;

typedef struct	s_sq
{
	t_list		*obj;
	t_list		*bot;
}				t_sq;

typedef struct	s_env
{
	t_opt		opt;
	int			maxfd;
	int			max;
	int			res;
	fd_set		fd_read;
	fd_set		fd_write;
	t_fd		*fds;
	int			srv;
	int			gfx_srv;
	int			bot_srv;
	t_list		*client_lst;
	t_list		*gfx_lst;
	int			n_sq;
	t_list		*team;
	t_list		*bot_fd_lst;
	t_sq		*board;
}				t_env;

t_buf			*buf_new(void);
int				buf_load(t_buf *buffer, char *str);
int				buf_unload(t_buf *buffer, char *res);
int				buf_len(t_buf *buffer);

int				try_int(int res, int err, char *str);
void			*try_void(void *res, void *err, char *str);

int				check_end(t_env *e);

int				sq_rand(t_env *e);
int				dir_rand();

void			get_opt(int ac, char **av, t_opt *opt);

void			fd_destroy(t_env *e, int fd, char *msg);
void			fd_clean(t_fd *fd);
void			fd_watch(t_env *e, int fd);
void			fd_check(t_env *e, int fd);
void			fd_iter_all(t_env *e, void (*fct)());

t_team			*team_new(char *name, int limit);
t_team			*get_team_by_name(t_env *e, char *name);

int				get_obj_type(char *obj_name);
t_obj			*get_obj(t_env *e, int sq, int type);
void			move_obj_random(t_env *e, t_obj *obj, int sq);
char			*get_obj_name(int type);
t_obj			*obj_new(int type);

t_bot			*bot_new(t_team *team);
void			bot_iter_all_connected_queued_egg(t_env *e, void (*fct)());
t_bot			*get_bot_by_fd(t_env *e, int fd);
t_bot			*get_bot_by_id(t_env *e, int id);
void			bot_destroy(t_env *e, int fd, char *msg);
t_bot			*get_bot_by_id_arg(t_env *e, int fd, char **req);

t_gfx			*gfx_new(int fd);
void			gfx_destroy(t_env *e, int fd, char *msg);

void			gfx_connection(t_env *e, int fd);
void			bot_connection(t_env *e, int fd, char *team_name);

void			client_read(t_env *e, int cs);
void			client_write(t_env *e, int cs);

void			init_connection(t_env *e);
void			init_game(t_env *e);
void			print_board(t_env *e);

void			bot_parse_request(t_env *e, int cs, char *str);
void			gfx_parse_request(t_env *e, int fd, char *str);

int				get_north(t_env *e, int sq);
int				get_south(t_env *e, int sq);
int				get_east(t_env *e, int sq);
int				get_west(t_env *e, int sq);

t_bot			*connect_bot(t_env *e, t_team *team);
void			unconnect_bot(t_env *e, t_bot *bot);

void			timer(t_env *e, t_bot *bot);

void			msz(t_env *e, int fd);
void			bct(t_env *e, int fd, char **req, int square);
void			mct(t_env *e, int fd);
void			tna(t_env *e, int fd);
void			ppo(t_env *e, int fd, char **req, t_bot *b);
void			plv(t_env *e, int fd, char **req, t_bot *b);
void			pin(t_env *e, int fd, char **req, t_bot *b);
void			sgt(t_env *e, int fd);
void			sst(t_env *e, int fd, char **req, int tmp);

void			pnw(t_env *e, int fd, t_bot *bot);
void			pex(t_env *e, int fd, t_bot *bot);
void			pbc(t_env *e, int fd, t_bot *bot, char *msg);
void			pic(t_env *e, int fd, t_bot *bot);
void			pie(t_env *e, int fd, t_bot *bot, int res);
void			pfk(t_env *e, int fd, t_bot *bot);
void			pdr(t_env *e, int fd, t_bot *bot, int type);
void			pgt(t_env *e, int fd, t_bot *bot, int type);
void			pdi(t_env *e, int fd, t_bot *bot);
void			enw(t_env *e, int fd, t_bot *bot, t_bot *new_bot);
void			eht(t_env *e, int fd, t_bot *bot);
void			ebo(t_env *e, int fd, t_bot *bot);
void			edi(t_env *e, int fd, t_bot *bot);
void			seg(t_env *e, int fd, t_team *team);
void			smg(t_env *e, int fd, char *msg);
void			suc(t_env *e, int fd);
void			sbp(t_env *e, int fd);

void			notify_all_gfx_pnw(t_env *e, t_bot *bot);
void			notify_all_gfx_ppo(t_env *e, t_bot *bot);
void			notify_all_gfx_take(t_env *e, t_bot *bot, int type);
void			notify_all_gfx_put(t_env *e, t_bot *bot, int type);
void			notify_all_gfx_pdi(t_env *e, t_bot *bot);
void			notify_all_gfx_pex(t_env *e, t_bot *bot);
void			notify_all_gfx_pbc(t_env *e, t_bot *bot, char *msg);
void			notify_all_gfx_pfk(t_env *e, t_bot *bot);
void			notify_all_gfx_enw(t_env *e, t_bot *bot, t_bot *egg);
void			notify_all_gfx_eht(t_env *e, t_bot *bot);
void			notify_all_gfx_edi(t_env *e, t_bot *bot);
void			notify_all_gfx_ebo(t_env *e, t_bot *bot);
void			notify_all_gfx_bct(t_env *e, int sq);
void			notify_all_gfx_pic(t_env *e, t_bot *bot);
void			notify_all_gfx_plv(t_env *e, t_bot *bot);
void			notify_all_gfx_pie(t_env *e, t_bot *bot, int res);
void			notify_all_gfx_seg(t_env *e, t_team *team);
void			notify_all_gfx_sgt(t_env *e);

void			broadcast(t_env *e, t_bot *bot, char *msg);
void			take(t_env *e, t_bot *bot, char *obj_name);
void			put(t_env *e, t_bot *bot, char *obj_name);
void			step(t_env *e, t_bot *bot);
void			turn_right(t_env *e, t_bot *bot);
void			turn_left(t_env *e, t_bot *bot);
void			look(t_env *e, t_bot *bot);
void			get_inventory(t_env *e, t_bot *bot);
void			expulse(t_env *e, t_bot *bot);
void			incantation(t_env *e, t_bot *bot);
void			fork_egg(t_env *e, t_bot *bot);
void			send_nbr(t_env *e, int fd);
void			move(t_env *e, t_bot *bot, int sq);
void			send_dimension(t_env *e, int fd);
int				*incant_get_requirements(int level);
void			incant_get_elements(t_env *e, t_bot *bot);
void			incant_del_elements(t_env *e, t_bot *bot);
void			move_rocks_after_incant(t_env *e, t_bot *bot);

#endif
