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

int				count_needed_routes(t_routes *routes)
{
	t_route		*start;
	t_route		*end;
	t_route		*room;
	t_route		*route;

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
		room = start;
		while (room->next && room->next->room->id != route->room->id)
			room = room->next;
		if (!room->next)
		{
			if (!(room->next = (t_route *)malloc(sizeof(t_route))))
				handle_error();
			room->next->room = route->room;
			printf("s: %s\n", room->next->room->name);
			room->next->next = NULL;
		}

		while (route->index != routes->rooms - 1)
			route = route->next;
		room = end;
		while (room->next && room->next->room->id != route->room->id)
			room = room->next;
		if (!room->next)
		{
			if (!(room->next = (t_route *)malloc(sizeof(t_route))))
				handle_error();
			room->next->room = route->room;
			printf("e: %s\n", room->next->room->name);
			room->next->next = NULL;
		}

		routes = routes->next;
	}
	printf("End of count_needed_routes\n");
	return (count_rooms(start, end));
}

static t_routes	*del_route(t_routes *routes)
{
	t_routes	*next;
	t_route		*route;
	t_route		*tmp;

	next = routes->next;
	route = routes->route;
	while (route)
	{
		tmp = route;
		route = route->next;
		free(tmp);
	}
	free(routes);
	return (next);
}

t_routes		*del_dead_ends(t_routes *route)
{
	t_routes	*head;
	t_routes	*previous;

	while (route && route->rooms == 0)
	{
		previous = route;
		route = del_route(previous);
	}
	if (!route)
		handle_error();
	head = route;
	previous = route;
	route = route->next;
	while (route)
	{
		if (route->rooms == 0)
			previous->next = del_route(route);
		else
			previous = previous->next;
		route = previous->next;
	}
	return (head);
}
