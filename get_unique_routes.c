#include "lemin.h"

static void		save_routes(t_routes **tmp, t_routes **unique, int size)
{
	int			*tmp_len;
	int			*unique_len;
	int			i;

	tmp_len = count_routes_len(tmp, size);
	unique_len = count_routes_len(unique, size);
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

static int		is_unique_route(t_route *route1, t_route *head)
{
	t_route		*route2;

	route1 = route1->next;
	while (route1->room->type == NORMAL)
	{
		route2 = head->next;
		while (route2->room->type == NORMAL)
		{
			if (route2->room->id == route1->room->id)
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
}

t_routes		**get_unique_routes(t_routes **routes)
{
	t_routes	**unique;
	t_routes	**tmp;
	int			size;
	int			i;

	size = count_words((void **)routes);
	unique = get_array_routes(size);
	tmp = get_array_routes(size);
	set_unique_routes(routes, 0, tmp, unique);
	free(tmp);
	unique = del_null_elems(unique, size);
	order_routes(unique);
	return (unique);
}
