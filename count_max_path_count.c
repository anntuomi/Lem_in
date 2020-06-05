/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_max_path_count.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuomine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/01 14:01:52 by atuomine          #+#    #+#             */
/*   Updated: 2020/06/01 14:01:57 by atuomine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static int		count_rooms(t_route *start, t_route *end)
{
	int			start_rooms;
	int			end_rooms;
	t_route		*room_start;
	t_route		*room_end;

	start_rooms = 0;
	end_rooms = 0;
	while (start || end)
	{
		if (start)
		{
			room_start = start->next;
			free(start);
			start = room_start;
			start_rooms++;
		}
		if (end)
		{
			room_end = end->next;
			free(end);
			end = room_end;
			end_rooms++;
		}
	}
	return (start_rooms < end_rooms ? start_rooms - 1 : end_rooms - 1);
}

static void		save_room(t_route *room, t_route *route)
{
	while (room->next && room->next->room->id != route->room->id)
		room = room->next;
	if (!room->next)
	{
		if (!(room->next = (t_route *)malloc(sizeof(t_route))))
			handle_error();
		room->next->room = route->room;
		room->next->next = NULL;
	}
}

/*
** Saving rooms connected to start to *start, and rooms connected to end to
** *end.
*/

int				count_max_path_count(t_routes *routes, t_routes *shortest)
{
	t_route		*start;
	t_route		*end;
	t_route		*route;

	if (shortest->rooms == 2)
		return (1);
	if (!(start = (t_route *)malloc(sizeof(t_route))) ||
	!(end = (t_route *)malloc(sizeof(t_route))))
		handle_error();
	start->next = NULL;
	end->next = NULL;
	while (routes)
	{
		route = routes->route;
		while (route->index != 2)
			route = route->next;
		save_room(start, route);
		while (route->index != routes->rooms - 1)
			route = route->next;
		save_room(end, route);
		routes = routes->next;
	}
	return (count_rooms(start, end));
}
