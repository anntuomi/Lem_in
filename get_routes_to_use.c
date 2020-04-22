#include "lemin.h"

static void		add_room(t_room *room, t_routes *routes)
{
	t_route		*route;

	route = routes->route;
	if (!route->room)
		route->room = room;
	else
	{
		while (route->next)
			route = route->next;
		route->next = get_route(room, route->index + 1);
		routes->rooms++;
	}
}

static int		is_dup(int id, t_route *route)
{
	while (route)
	{
		if (route->room && route->room->id == id)
			return (1);
		route = route->next;
	}
	return (0);
}

static t_routes	*get_start_end_rooms(t_routes **routes)
{
	t_routes	*start_rooms;
	t_routes	*end_rooms;
	t_route		*route;
	int			i;

	start_rooms = get_routes(NULL);
	end_rooms = get_routes(NULL);
	start_rooms->next = end_rooms;
	i = 0;
	while (routes[i])
	{
		route = routes[i]->route;
		while (route)
		{
			if (route->index == 2 &&
			!is_dup(route->room->id, start_rooms->route))
				add_room(route->room, start_rooms);
			if (route->index == routes[i]->rooms - 1 &&
			!is_dup(route->room->id, end_rooms->route))
				add_room(route->room, end_rooms);
			route = route->next;
		}
		i++;
	}
	return (start_rooms);
}

t_routes		**get_routes_array(int size)
{
	t_routes	**array_routes;
	int			i;

	if (!(array_routes = (t_routes **)malloc(sizeof(t_routes *) * (size + 1))))
		handle_error();
	i = 0;
	while (i <= size)
		array_routes[i++] = NULL;
	return (array_routes);
}

t_routes		**get_routes_to_use(t_farm farm)
{
	t_routes	*start_rooms;
	t_routes	*end_rooms;
	t_routes	**rooms_routes;
	t_routes	**unique_routes;

	if (farm.amount == 1)
	{
		unique_routes = get_routes_array(1);
		unique_routes[0] = farm.ordered[0];
	}
	else
	{
		start_rooms = get_start_end_rooms(farm.ordered);
		end_rooms = start_rooms->next;
		//print_start_end_rooms(start_rooms);
		if (start_rooms->rooms <= end_rooms->rooms)
			rooms_routes = get_rooms_routes(start_rooms, 1, farm.ordered);
		else
			rooms_routes = get_rooms_routes(end_rooms, 0, farm.ordered);
		del_route(start_rooms);
		del_route(end_rooms);
		//print_rooms_routes(rooms_routes);
		unique_routes = get_unique_routes(rooms_routes);
		del_routes_linked_list_array(rooms_routes);
	}
	//print_unique_routes(unique_routes);
	return (unique_routes);
}
