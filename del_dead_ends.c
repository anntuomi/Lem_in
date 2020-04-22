#include "lemin.h"

t_routes		*del_route(t_routes *routes)
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
