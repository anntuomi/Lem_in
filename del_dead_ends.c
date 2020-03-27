#include "lemin.h"

static void	del_route(t_routes *routes)
{
	t_route	*route;
	t_route	*tmp;

	route = routes->route;
	while (route)
	{
		tmp = route;
		route = route->next;
		free(tmp);
	}
	free(routes);
}

t_routes	*del_dead_ends(t_routes *routes)
{
	t_routes	*head;
	t_routes	*previous;

	while (routes && routes->rooms == 0)
	{
		previous = routes;
		routes = routes->next;
		del_route(previous);
	}
	if (!routes)
		handle_error();
	head = routes;
	previous = routes;
	routes = routes->next;
	while (routes)
	{
		if (routes->rooms == 0)
		{
			previous->next = routes->next;
			del_route(routes);
		}
		previous = routes;
		routes = previous->next;
	}
	return (head);
}
