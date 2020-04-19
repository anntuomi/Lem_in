#include "lemin.h"

static int		count_words(t_routes **array)
{
	int		words;

	words = 0;
	while (array[words])
		words++;
	return (words);
}

static void		print_null_routes(t_routes **routes, int size)
{
	t_route		*route;
	int			i;

	//printf("\nROUTES\n");
	i = 0;
	while (i < size)
	{
		if (!routes[i])
			printf("%d. route NULL\n", i + 1);
		else
		{
			//printf("%d. route (%d rooms)\n", i + 1, routes[i]->rooms);
			route = routes[i]->route;
			while (route)
			{
				//printf("%d. %s\n", route->index, route->room->name);
				route = route->next;
			}
		}
		i++;
	}
}

static void		print_unique_routes(t_routes **routes)
{
	t_route		*route;
	int			i;

	//printf("\nUNIQUE ROUTES\n");
	i = 0;
	while (routes[i])
	{
		//printf("%d. route (%d rooms)\n", i + 1, routes[i]->rooms);
		route = routes[i]->route;
		while (route)
		{
			//printf("%d. %s\n", route->index, route->room->name);
			route = route->next;
		}
		i++;
	}
}

static int		*count_route_len(t_routes **routes, int size)
{
	int			*len;
	int			i;

	if (!(len = (int *)malloc(sizeof(int) * 2)))
		handle_error();
	len[0] = 0;
	len[1] = 0;
	i = 0;
	while (i < size)
	{
		if (routes[i])
		{
			len[0]++;
			len[1] += routes[i]->rooms;
		}
		i++;
	}
	return (len);
}

static int		is_unique_route(t_route *route1, t_route *head)
{
	t_route		*route2;

	route1 = route1;
	while (route1)
	{
		route2 = head;
		while (route1->room->type == NORMAL && route2)
		{
			if (route2->room->type == NORMAL &&
			route2->room->id == route1->room->id)
				return (0);
			route2 = route2->next;
		}
		route1 = route1->next;
	}
	return (1);
}

static int		are_unique_routes(t_routes **routes, int size)
{
	int			i1;
	int			i2;

	i1 = 0;
	while (i1 < size)
	{
		i2 = i1 + 1;
		while (i2 < size)
		{
			if (routes[i1] && routes[i2] && !is_unique_route(routes[i1]->route,
			routes[i2]->route))
				return (0);
			i2++;
		}
		i1++;
	}
	return (1);
}

static void		print_rooms_routes(t_routes **rooms_routes)
{
	t_routes	*routes;
	t_route		*route;
	int			i;

	//printf("\nrooms routes\n");
	i = 0;
	while (rooms_routes[i])
	{
		//printf("%d. start/end rooms routes\n", i + 1);
		routes = rooms_routes[i];
		while (routes)
		{
			//printf("route (%d rooms)\n", routes->rooms);
			route = routes->route;
			while (route)
			{
				//printf("%d. %s\n", route->index, route->room->name);
				route = route->next;
			}
			routes = routes->next;
		}
		i++;
	}
}

static t_routes	*get_routes_to_route(t_route *route, int rooms)
{
	t_routes	*routes;

	if (!(routes = (t_routes *)malloc(sizeof(t_routes))))
		handle_error();
	routes->route = route;
	routes->rooms = rooms;
	routes->next = NULL;
	return (routes);
}

static void		add_route(t_routes **dst, int i, t_routes *src)
{
	t_routes	*routes;

	if (!dst[i])
		dst[i] = get_routes_to_route(src->route, src->rooms);
	else
	{
		routes = dst[i];
		while (routes->next)
			routes = routes->next;
		routes->next = get_routes_to_route(src->route, src->rooms);
	}
}

static t_routes	**get_array_routes(int size)
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

static void		save_routes(t_routes **tmp, t_routes **unique, int size)
{
	int			*tmp_len;
	int			*unique_len;
	int			i;

	tmp_len = count_route_len(tmp, size);
	unique_len = count_route_len(unique, size);
	if (tmp_len[0] > unique_len[0] ||
	(tmp_len[0] == unique_len[0] && tmp_len[1] < unique_len[1]))
	{
		i = 0;
		while (i < size)
		{
			unique[i] = tmp[i];
			i++;
		}
	}
	free(tmp_len);
	free(unique_len);
}

static void		set_unique_routes(t_routes **routes, int room, t_routes **tmp,
t_routes **unique)
{
	t_routes	*route;

	route = routes[room];
	while (1)
	{
		tmp[room] = route;
		if (routes[room + 1])
			set_unique_routes(routes, room + 1, tmp, unique);
		else if (are_unique_routes(tmp, room + 1))
			save_routes(tmp, unique, room + 1);
		if (!route)
			break ;
		route = route->next;
	}
	tmp[room] = routes[room];
}

static void		order_routes(t_routes **routes)
{
	t_routes	*tmp;
	int			i;
	int			j;

	i = 0;
	while (routes[i])
	{
		j = i + 1;
		while (routes[j])
		{
			if (routes[j]->rooms < routes[i]->rooms)
			{
				tmp = routes[i];
				routes[i] = routes[j];
				routes[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static t_routes	**del_null_elems(t_routes **routes, int size)
{
	t_routes	**new;
	int			*new_size;
	int			i1;
	int			i2;

	new_size = count_route_len(routes, size);
	if (!(new = (t_routes **)malloc(sizeof(t_routes *) * (new_size[0] + 1))))
		handle_error();
	i1 = 0;
	i2 = 0;
	while (i1 < size)
	{
		if (routes[i1])
		{
			new[i2++] = routes[i1];
		}
		i1++;
	}
	new[i2] = NULL;
	free(routes);
	return (new);
}

static t_routes	**get_unique_routes(t_routes **routes)
{
	t_routes	**unique;
	t_routes	**tmp;
	int			size;
	int			i;

	size = count_words(routes);
	unique = get_array_routes(size);
	tmp = get_array_routes(size);
	set_unique_routes(routes, 0, tmp, unique);
	free(tmp);
	unique = del_null_elems(unique, size);
	order_routes(unique);
	return (unique);
}

static void		set_index(int *index, t_route *route, t_routes *rooms,
int start)
{
	t_route		*room;

	room = rooms->route;
	while (room && room->room->id != route->room->id)
		room = room->next;
	if (route->index == start)
		*index = room->index;
	else if (room)
		*index = 0;
}

static t_routes	**get_rooms_routes(t_routes *rooms, int start,
t_routes **routes)
{
	t_routes	**rooms_routes;
	t_route		*route;
	int			index;
	int			i;

	rooms_routes = get_array_routes(rooms->rooms);
	i = 0;
	while (routes[i])
	{
		index = -1;
		route = routes[i]->route;
		while (route)
		{
			set_index(&index, route, rooms, start ? 2 : routes[i]->rooms - 1);
			if (!index)
				break ;
			route = route->next;
		}
		if (index)
			add_route(rooms_routes, index - 1, routes[i]);
		i++;
	}
	rooms_routes = del_null_elems(rooms_routes, rooms->rooms);
	return (rooms_routes);
}

static void		print_start_end_rooms(t_routes *start_end_rooms)
{
	t_routes	*routes;
	t_route		*route;

	routes = start_end_rooms;
	//printf("\nstart rooms %d\n", routes->rooms);
	route = routes->route;
	while (route)
	{
		if (route->room)
			//printf("%d. room %s\n", route->index, route->room->name);
		route = route->next;
	}
	routes = routes->next;
	//printf("end rooms %d\n", routes->rooms);
	route = routes->route;
	while (route)
	{
		if (route->room)
			//printf("%d. room %s\n", route->index, route->room->name);
		route = route->next;
	}
}

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

static t_routes	*get_start_end_rooms(t_routes *routes)
{
	t_routes	*start_rooms;
	t_routes	*end_rooms;
	t_route		*route;

	start_rooms = get_routes(NULL);
	end_rooms = get_routes(NULL);
	start_rooms->next = end_rooms;
	while (routes)
	{
		route = routes->route;
		while (route)
		{
			if (route->index == 2 &&
			!is_dup(route->room->id, start_rooms->route))
				add_room(route->room, start_rooms);
			if (route->index == routes->rooms - 1 &&
			!is_dup(route->room->id, end_rooms->route))
				add_room(route->room, end_rooms);
			route = route->next;
		}
		routes = routes->next;
	}
	return (start_rooms);
}

static void		mv_ants(t_farm farm, t_routes **unique_routes)
{
	int			needed_routes;
	int			i1;
	int			i2;

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

void			ants_to_end(t_farm farm)
{
	t_routes	*start_rooms;
	t_routes	*end_rooms;
	t_routes	**rooms_routes;
	t_routes	**unique_routes;
	t_room		*room;

	if (farm.amount == 1)
	{
		unique_routes = get_array_routes(1);
		unique_routes[0] = farm.ordered[0];
	}
	else
	{
		start_rooms = get_start_end_rooms(farm.routes);
		end_rooms = start_rooms->next;
		print_start_end_rooms(start_rooms);
		if (start_rooms->rooms <= end_rooms->rooms)
			rooms_routes = get_rooms_routes(start_rooms, 1, farm.ordered);
		else
			rooms_routes = get_rooms_routes(end_rooms, 0, farm.ordered);
		print_rooms_routes(rooms_routes);
		unique_routes = get_unique_routes(rooms_routes);
	}
	print_unique_routes(unique_routes);
	room = farm.rooms;
	while (room)
	{
		room->ant_count = 0;
		room = room->next;
	}
	printf("\n");
	mv_ants(farm, unique_routes);
}
