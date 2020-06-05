/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_routes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuomine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/01 14:02:54 by atuomine          #+#    #+#             */
/*   Updated: 2020/06/01 14:02:56 by atuomine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static int		is_connected_to_end(t_path *path, int *paths, t_room **room,
t_room **prev)
{
	int			end;
	t_room		*path_room;

	end = 0;
	while (path)
	{
		path_room = path->room;
		if (path_room->type == END)
		{
			*prev = *room;
			*room = path_room;
			end = 1;
		}
		*paths += 1;
		path = path->next;
	}
	return (end);
}

static void		set_fork_routes(t_room **room, t_room **prev, t_route *route,
int *paths)
{
	t_path		*path;
	t_room		*bef;
	int			first;

	path = (*room)->paths;
	bef = *prev;
	if (is_connected_to_end(path, paths, room, prev))
		return ;
	first = 1;
	while (path)
	{
		if (is_unvisited(path->room, bef, route->forks))
		{
			if (first)
			{
				*prev = *room;
				*room = path->room;
				first = 0;
			}
			else
				set_routes(route, path->room, *prev);
		}
		path = path->next;
	}
	*paths = (first ? 0 : *paths);
}

static t_fork	*get_current_route(t_route **route, t_route **prev_route,
t_room *room, t_room *prev_room)
{
	t_fork		*fork;
	t_route		*before_fork;

	fork = NULL;
	if (room->type != START)
	{
		before_fork = *route;
		while ((*route)->next)
			*route = (*route)->next;
		*prev_route = *route;
		(*route)->next = get_route();
		*route = (*route)->next;
		set_route_before_fork(*route, &fork, before_fork);
		set_fork(*route, &fork, prev_room, room);
		(*route)->rooms++;
	}
	return (fork);
}

void			set_routes(t_route *route, t_room *room, t_room *prev_room)
{
	t_fork		*fork;
	int			paths;
	t_route		*prev_route;

	prev_route = NULL;
	fork = get_current_route(&route, &prev_route, room, prev_room);
	while (room && room->type != END)
	{
		paths = 0;
		set_fork_routes(&room, &prev_room, route, &paths);
		if ((prev_room->type == START && paths > 1) || paths > 2)
			set_fork(route, &fork, prev_room, room);
		else if (!paths)
		{
			route->rooms = 0;
			break ;
		}
		route->rooms++;
	}
}

t_route			*get_routes(t_room *start)
{
	t_route		*route;

	route = get_route();
	set_routes(route, start, start);
	route = del_dead_ends(route);
	return (route);
}
