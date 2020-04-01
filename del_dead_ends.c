#include "lemin.h"

static int		count_rooms(t_route *head)
{
	int			rooms;
	t_route		*room;

	rooms = 0;
	while (head)
	{
		room = head->next;
		free(head);
		head = room;
		rooms++;
	}
	return (rooms - 1);
}

int				count_rooms_to_end(t_routes *routes)
{
	t_route		*head;
	t_route		*room;
	t_route		*route;

	if (!(head = (t_route *)malloc(sizeof(t_route))))
		handle_error();
	head->next = NULL;
	while (routes)
	{
		route = routes->route;
		while (route->index != routes->rooms - 1)
			route = route->next;
		room = head;
		while (room->next && room->next->room->id != route->room->id)
			room = room->next;
		if (!room->next)
		{
			if (!(room->next = (t_route *)malloc(sizeof(t_route))))
				handle_error();
			room->next->room = route->room;
			room->next->next = NULL;
		}
		routes = routes->next;
	}
	return (count_rooms(head));
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
