#include "lemin.h"

static int		is_unique(t_routes **used_list, t_route *tested_head)
{
	t_route		*tested_route;
	t_route		*used_route;
	int			i;
	int			tested_room;

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

static t_routes	**save_most_uniques(t_routes **used, t_routes **most_uniques,
int j, int *most_paths)
{
	int			i;

	if (*most_paths < j)
	{
		i = 0;
		while (used[i])
		{
			most_uniques[i] = used[i];
			i++;
		}
		*most_paths = j;
	}
	return (most_uniques);
}

static t_routes	*find_next_unique(int i, t_routes **used_routes,
t_routes **ordered)
{
	while (ordered[i])
	{
		if (is_unique(used_routes, ordered[i]->route) == 1)
			return (ordered[i]);
		i++;
	}
	return (NULL);
}

static t_routes	**return_most_paths(t_routes **used_routes,
t_routes **most_uniques, int path_count, int j)
{
	//printf("End of determine used\n");
	if (j == path_count)
	{
		free(most_uniques);
		return (used_routes);
	}
	else
	{
		free(used_routes);
		return (most_uniques);
	}
}

t_routes		**determine_used_routes(t_routes **ordered, int *path_count,
int i, int j)
{
	t_routes	**most_uniques;
	t_routes	**used_routes;
	int			most_paths;
	int			orig_path_count;

	initialize_arrays(*path_count, &used_routes, &most_uniques, &most_paths);
	while (ordered[i] && j < *path_count)
	{
		used_routes[0] = ordered[i];
		j = 1;
		while (j < *path_count)
		{
			used_routes[j] = find_next_unique(i + 1, used_routes, ordered);
			if (used_routes[j] == NULL)
			{
				save_most_uniques(used_routes, most_uniques, j, &most_paths);
				break ;
			}
			j++;
		}
		i++;
	}
	orig_path_count = *path_count;
	*path_count = (j != *path_count ? most_paths : *path_count);
	return (return_most_paths(used_routes, most_uniques, orig_path_count, j));
}
