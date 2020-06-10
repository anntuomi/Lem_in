/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_route.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuomine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/01 14:02:46 by atuomine          #+#    #+#             */
/*   Updated: 2020/06/01 14:02:48 by atuomine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void			del_route(t_route **route, t_route *prev)
{
	t_route		*next;
	t_fork		*fork;
	t_fork		*tmp;

	next = (*route)->next;
	if (prev)
		prev->next = next;
	fork = (*route)->forks;
	while (fork)
	{
		tmp = fork;
		fork = fork->next;
		free(tmp);
	}
	free(*route);
	*route = NULL;
}

int				is_unvisited(t_room *room, t_room *prev, t_fork *fork)
{
	if (prev != room)
	{
		while (fork)
		{
			if (fork->from->id == room->id)
				return (0);
			fork = fork->next;
		}
		return (1);
	}
	return (0);
}

void			set_fork(t_route *route, t_fork **fork, t_room *from,
t_room *to)
{
	t_fork		*new;

	if (!(new = (t_fork *)malloc(sizeof(t_fork))))
		handle_error();
	new->from = from;
	new->to = to;
	new->next = NULL;
	if (!*fork)
	{
		*fork = new;
		route->forks = *fork;
	}
	else
	{
		(*fork)->next = new;
		*fork = (*fork)->next;
	}
}

void			set_route_before_fork(t_route *route, t_fork **fork,
t_route *before_fork)
{
	t_fork		*bf;

	bf = before_fork->forks;
	while (bf)
	{
		set_fork(route, fork, bf->from, bf->to);
		bf = bf->next;
	}
	route->rooms = before_fork->rooms;
}

t_route			*get_route(void)
{
	t_route		*route;

	if (!(route = (t_route *)malloc(sizeof(t_route))))
		handle_error();
	route->forks = NULL;
	route->rooms = 1;
	route->next = NULL;
	return (route);
}
