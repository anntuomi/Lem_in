#include "lemin.h"

void			order_routes(t_routes **routes)
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

t_routes		**routes_to_array(int route_count, t_routes *routes)
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

int				count_routes(t_routes *routes)
{
	int			count;

	count = 0;
	while (routes)
	{
		routes = routes->next;
		count++;
	}
	return (count);
}
