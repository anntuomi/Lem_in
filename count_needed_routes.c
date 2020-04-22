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
		if (start != NULL)
		{
			room_start = start->next;
			free(start);
			start = room_start;
			start_rooms++;
		}
		if (end != NULL)
		{
			room_end = end->next;
			free(end);
			end = room_end;
			end_rooms++;
		}
	}
	return (start_rooms < end_rooms ? start_rooms - 1 : end_rooms - 1);
}

static void		save_start_room(t_route *start, t_route *route)
{
	t_route		*room;

	room = start;
	while (room->next && room->next->room->id != route->room->id)
		room = room->next;
	if (!room->next)
	{
		if (!(room->next = (t_route *)malloc(sizeof(t_route))))
			handle_error();
		room->next->room = route->room;
		//printf("s: %s\n", room->next->room->name);
		room->next->next = NULL;
	}
}

static void		save_end_room(t_route *end, t_route *route)
{
	t_route		*room;

	room = end;
	while (room->next && room->next->room->id != route->room->id)
		room = room->next;
	if (!room->next)
	{
		if (!(room->next = (t_route *)malloc(sizeof(t_route))))
			handle_error();
		room->next->room = route->room;
		//printf("e: %s\n", room->next->room->name);
		room->next->next = NULL;
	}
}

int				count_needed_routes(t_routes *routes, t_routes *shortest)
{
	t_route		*start;
	t_route		*end;
	t_route		*room;
	t_route		*route;

	if (shortest->rooms == 2)
		return (1);
	if (!(end = (t_route *)malloc(sizeof(t_route))) || \
		!(start = (t_route *)malloc(sizeof(t_route))))
		handle_error();
	end->next = NULL;
	start->next = NULL;
	while (routes)
	{
		route = routes->route;
		while (route->index != 2)
			route = route->next;
		save_start_room(start, route);
		while (route->index != routes->rooms - 1)
			route = route->next;
		save_end_room(end, route);
		routes = routes->next;
	}
	return (count_rooms(start, end));
}
