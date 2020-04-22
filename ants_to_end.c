#include "lemin.h"

void			print_null_routes(t_routes **routes, int size)
{
	t_route		*route;
	int			i;

	printf("\nnull routes\n");
	i = 0;
	while (i < size)
	{
		if (!routes[i])
			printf("%d. route NULL\n", i + 1);
		else
		{
			printf("%d. route (%d rooms)\n", i + 1, routes[i]->rooms);
			route = routes[i]->route;
			while (route)
			{
				printf("%d. %s\n", route->index, route->room->name);
				route = route->next;
			}
		}
		i++;
	}
}

void			print_unique_routes(t_routes **routes)
{
	t_route		*route;
	int			i;

	printf("\nunique routes\n");
	i = 0;
	while (routes[i])
	{
		printf("%d. route (%d rooms)\n", i + 1, routes[i]->rooms);
		route = routes[i]->route;
		while (route)
		{
			printf("%d. %s\n", route->index, route->room->name);
			route = route->next;
		}
		i++;
	}
}

void			print_rooms_routes(t_routes **rooms_routes)
{
	t_routes	*routes;
	t_route		*route;
	int			i;

	printf("\nrooms routes\n");
	i = 0;
	while (rooms_routes[i])
	{
		printf("%d. routes\n", i + 1);
		routes = rooms_routes[i];
		while (routes)
		{
			printf("route (%d rooms)\n", routes->rooms);
			route = routes->route;
			while (route)
			{
				printf("%d. %s\n", route->index, route->room->name);
				route = route->next;
			}
			routes = routes->next;
		}
		i++;
	}
}

void			print_start_end_rooms(t_routes *start_end_rooms)
{
	t_routes	*routes;
	t_route		*route;

	routes = start_end_rooms;
	printf("\nstart rooms %d\n", routes->rooms);
	route = routes->route;
	while (route)
	{
		if (route->room)
			printf("%d. %s\n", route->index, route->room->name);
		route = route->next;
	}
	routes = routes->next;
	printf("end rooms %d\n", routes->rooms);
	route = routes->route;
	while (route)
	{
		if (route->room)
			printf("%d. %s\n", route->index, route->room->name);
		route = route->next;
	}
}

void			ants_to_end(t_farm farm, t_routes **unique_routes)
{
	t_room		*room;
	int			needed_routes;
	int			i1;
	int			i2;

	room = farm.rooms;
	while (room)
	{
		room->ant_count = 0;
		room = room->next;
	}
	needed_routes = 0;
	i1 = 0;
	while (unique_routes[i1] && (needed_routes < farm.amount ||
	unique_routes[i1]->rooms == unique_routes[i1 - 1]->rooms))
		needed_routes += unique_routes[i1++]->rooms;
	needed_routes = i1;
	i1 = 0;
	i2 = 0;
	while (i1 < farm.amount)
	{
		if (i2 >= needed_routes)
			i2 = 0;
		farm.ants[i1] = unique_routes[i2++]->route;
		farm.ants[i1++]->room->ant_count++;
	}
	farm.ants[i1] = NULL;
	needed_routes = 0;
	printf("\n");
	while (farm.end->ant_count != farm.amount)
	{
		i1 = 0;
		i2 = 1;
		while (farm.ants[i1])
		{
			if (farm.ants[i1]->room->type != END &&
			(!farm.ants[i1]->next->room->ant_count ||
			farm.ants[i1]->next->room->type == END))
			{
				farm.ants[i1]->room->ant_count--;
				farm.ants[i1] = farm.ants[i1]->next;
				farm.ants[i1]->room->ant_count++;
				if (!i2)
					printf(" ");
				else
					i2 = 0;
				printf("L%d-%s", i1 + 1, farm.ants[i1]->room->name);
			}
			i1++;
		}
		printf("\n");
		needed_routes++;
	}
	printf("Moves: %d\n", needed_routes);
}
