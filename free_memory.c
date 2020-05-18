#include "lemin.h"

static void		free_rooms(t_room *room)
{
	t_room *next_room;

	while (room)
	{
		next_room = room->next;
		free(room->name);
		free(room->paths);
		free(room);
		room = next_room;
	}
}

static void		free_current_route(t_route *route)
{
	t_route *next_route;

	while (route)
	{
		next_route = route->next;
		free(route);
		route = next_route;
	}
}

static void		free_routes(t_routes **ordered)
{
	int i;

	i = 0;
	while (ordered[i])
	{
		free_current_route(ordered[i]->route);
		free(ordered[i]);
		i++;
	}
	free(ordered);
}

void			free_memory(t_farm farm)
{
	free_rooms(farm.rooms);
	free(farm.ants);
	free_routes(farm.ordered);
}
