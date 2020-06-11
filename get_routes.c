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
	int			first;
	t_room		*new;

	path = (*room)->paths;
	if (is_connected_to_end(path, paths, room, prev))
		return ;
	first = 1;
	while (path)
	{
		if (is_unvisited(path->room, (*room)->type, *prev, route->forks))
		{
			if (first)
			{
				new = path->room;
				first = 0;
			}
			else
				set_routes(route, path->room, *room);
		}
		path = path->next;
	}
	*prev = *room;
	*room = new;
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
			if (!prev_route)
				route->rooms = 0;
			else
				del_route(&route, prev_route);
			break ;
		}
		route->rooms++;
	}
}

t_route			*get_routes(t_room *start)
{
	t_path		*path;
	t_room		*room;
	t_route		*route;
	t_route		*tmp;

	path = start->paths;
	while (path)
	{
		room = path->room;
		room->start_connection = 1;
		path = path->next;
	}
	route = get_route();
	set_routes(route, start, start);
	if (!route->rooms)
	{
		tmp = route;
		if (!(route = route->next))
			handle_error();
		del_route(&tmp, NULL);
	}
	return (route);
}
