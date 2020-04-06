#include "lemin.h"

static t_routes	**routes_to_array(int route_count, t_routes *routes)
{
	t_routes	**array_routes;
	t_routes	*current;
	int			i;

	i = 0;
	if (!(array_routes = (t_routes **)malloc(sizeof(t_routes *) *
	(route_count + 1))))
		handle_error();
	current = routes;
	while (current)
	{
		array_routes[i] = current;
		i++;
		current = current->next;
	}
	array_routes[route_count] = NULL;
	return (array_routes);
}

t_routes		**order_routes_shortest(int route_count, t_routes *routes)
{
	t_routes	**array_routes;
	t_routes	*tmp;
	int			i;
	int			j;

	array_routes = routes_to_array(route_count, routes);
	i = 0;
	while (i < route_count)
	{
		j = i + 1;
		while (j < route_count)
		{
			if (array_routes[j]->rooms < array_routes[i]->rooms)
			{
				tmp = array_routes[i];
				array_routes[i] = array_routes[j];
				array_routes[j] = tmp;
			}
			j++;
		}
		i++;
	}
	return (array_routes);
}
