/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_routes_to_end.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuomine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/01 14:02:54 by atuomine          #+#    #+#             */
/*   Updated: 2020/06/01 14:02:56 by atuomine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static void		set_fork_routes(t_path *path, t_routes *routes)
{
	int			first;

	first = 1;
	while (path)
	{
		if (is_unvisited(path->room, routes->route))
		{
			if (!first)
				set_routes(routes, path->room);
			else
				first = 0;
		}
		path = path->next;
	}
}

static t_route	*get_current_route(t_routes **routes, t_room *room)
{
	t_route		*route;
	t_routes	*before_fork;

	if (room->type != START)
	{
		before_fork = *routes;
		while ((*routes)->next)
			*routes = (*routes)->next;
		(*routes)->next = get_routes(before_fork->route->room);
		*routes = (*routes)->next;
		set_route_before_fork(*routes, before_fork);
		route = (*routes)->route;
		while (route->next)
			route = route->next;
		route->next = get_route(room, route->index + 1);
		route = route->next;
		(*routes)->rooms++;
	}
	else
		route = (*routes)->route;
	return (route);
}

void			set_routes(t_routes *routes, t_room *room)
{
	t_route		*route;
	t_path		*path;
	int			unvisited;

	route = get_current_route(&routes, room);
	while (room && room->type != END)
	{
		path = room->paths;
		if ((unvisited = count_unvisited(path, routes->route)) > 1)
			set_fork_routes(path, routes);
		if (unvisited)
		{
			while (!is_unvisited(path->room, routes->route))
				path = path->next;
			room = path->room;
			route->next = get_route(room, route->index + 1);
			route = route->next;
			routes->rooms++;
		}
		else
		{
			routes->rooms = 0;
			break ;
		}
	}
}

t_routes		*get_routes_to_end(t_room *start)
{
	t_routes	*routes;

	routes = get_routes(start);
	set_routes(routes, start);
	routes = del_dead_ends(routes);
	return (routes);
}
