#include "lemin.h"

static int		is_unique(t_route *tested_route, t_routes **used_routes)
{
	t_route		*used_route;
	int			tested_room;
	int			i;

	tested_route = tested_route->next;
	while (tested_route->room->type != END)
	{
		tested_room = tested_route->room->id;
		i = 0;
		while (used_routes[i])
		{
			used_route = used_routes[i]->route->next;
			while (used_route->room->type != END)
			{
				if (used_route->room->id == tested_room)
					return (0);
				used_route = used_route->next;
			}
			i++;
		}
		tested_route = tested_route->next;
	}
	return (1);
}

t_routes		*find_next_unique(t_routes **ordered, t_routes **used_routes)
{
	int			i;

	i = 0;
	while (ordered[i])
	{
		if (is_unique(ordered[i]->route, used_routes))
			return (ordered[i]);
		i++;
	}
	return (NULL);
}
