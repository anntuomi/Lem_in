#include "lemin.h"

static int		is_unique(t_routes **used_list, t_route *tested_head)
{
	t_route		*tested_route;
	t_route		*used_route;
	int			tested_room;
	int			i;

	tested_route = tested_head->next;
	while (tested_route->room->type != END)
	{
		i = 0;
		tested_room = tested_route->room->id;
		while (used_list[i])
		{
			used_route = used_list[i]->route->next;
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

t_routes		*find_next_unique(int i, t_routes **used_routes,
t_routes **ordered)
{
	while (ordered[i])
	{
		if (is_unique(used_routes, ordered[i]->route))
			return (ordered[i]);
		i++;
	}
	return (NULL);
}
