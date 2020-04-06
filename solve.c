#include "lemin.h"

static int		compare_routes(t_routes **used_list, t_route *tested_head)
{
	t_route	*tested_route;
	t_route	*used_route;
	int		i;
	int		tested_room;

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
	int i;

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

static t_routes	**determine_used_routes(t_routes **ordered, int *path_count)
{
	t_routes	**used_routes;
	t_routes	**most_uniques;
	int			most_paths;
	int			i;
	int			j;
	int			k;

	most_paths = -1;
	used_routes = (t_routes **)malloc(sizeof(t_routes *) * *path_count + 1);
	most_uniques = (t_routes **)malloc(sizeof(t_routes *) * *path_count + 1);
	i = 0;
	while (i <= *path_count)
	{
		used_routes[i] = NULL;
		most_uniques[i] = NULL;
		i++;
	}
	i = 0;
	while (ordered[i])
	{
		used_routes[0] = ordered[i];
		j = 1;
		while (j < *path_count)
		{
			k = 0;
			while (ordered[k])
			{
				if (compare_routes(used_routes, ordered[k]->route) == 1)
				{
					used_routes[j] = ordered[k];
					break ;
				}
				k++;
			}
			if (used_routes[j] == NULL)
			{
				save_most_uniques(used_routes, most_uniques, j, &most_paths);
				break ;
			}
			j++;
		}
		if (j == *path_count)
		{
			free(most_uniques);
			return (used_routes);
		}
		i++;
	}
	*path_count = most_paths;
	free(used_routes);
	return (most_uniques);
}

static t_route	**assign_paths(t_route **ant, t_routes **ordered,
int path_count, int amount)
{
	int			i;

	i = 0;
	while (i < amount)
	{
		printf("Ant number %d (Path_count: %d) Mod: %d\n", i, path_count,
		(i) % path_count);
		ant[i] = ordered[(i) % path_count]->route;
		i++;
	}
	return (ant);
}

static void		move_ants(int amount, t_route **ant, t_routes *route_list)
{
	int			i;
	int			moved;
	t_routes	*route;
	int			ant_nbr;

	i = amount - 1;
	ant_nbr = 1;
	moved = 0;
	while (i >= 0)
	{
		if (ant[i]->room->type != END && (ant[i]->next->room->ant_count == 0 \
		|| ant[i]->next->room->type == END))
		{
			ant[i]->room->ant_count--;
			ant[i] = ant[i]->next;
			ant[i]->room->ant_count++;
			moved == 1 ? printf(" L%d-%s", ant_nbr, ant[i]->room->name) : \
			printf("L%d-%s", ant_nbr, ant[i]->room->name);
			moved = 1;
		}
		i--;
		ant_nbr++;
	}
	printf("\n");
}

void			solve(t_farm farm, t_routes **ordered, int path_count)
{
	t_routes	**used_routes;
	int			moves;

	used_routes = determine_used_routes(ordered, &path_count);
	assign_paths(farm.ants, used_routes, path_count, farm.amount);
	moves = 0;
	while (farm.end->ant_count != farm.amount)
	{
		move_ants(farm.amount, farm.ants, farm.routes);
		moves++;
	}
	printf("Moves: %d\n", moves);
}
