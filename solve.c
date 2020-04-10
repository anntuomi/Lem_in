#include "lemin.h"

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

int				adjust_to_ant_amount(int path_count, int ant_amount,
t_routes **ordered, t_routes **used)
{
	int i;
	int capacity;
	int temp;

	if (ant_amount <= ordered[0]->rooms - 2 && \
	ordered[1]->rooms > ordered[0]->rooms)
		return (-1);
	i = 0;
	capacity = 0;
	while (i < path_count)
	{
		capacity = capacity + (used[i]->rooms - 2);
		printf("Capacity: %d\n", capacity);
		if (capacity >= ant_amount)
			break ;
		i++;
	}
	if (i < path_count)
	{
		temp = used[i]->rooms;
		while (i < path_count && used[i]->rooms == temp)
			i++;
	}
	return (i < path_count ? i : path_count);
}

void			solve(t_farm farm, t_routes **ordered, int path_count)
{
	t_routes	**used_routes;
	int			moves;

	used_routes = determine_used_routes(ordered, &path_count, 0, 0);
	if (path_count > 1)
		path_count = adjust_to_ant_amount(path_count, farm.amount,
	ordered, used_routes);
	if (path_count == -1)
		assign_paths(farm.ants, ordered, 1, farm.amount);
	else
		assign_paths(farm.ants, used_routes, path_count, farm.amount);
	moves = 0;
	while (farm.end->ant_count != farm.amount)
	{
		move_ants(farm.amount, farm.ants, farm.routes);
		moves++;
	}
	printf("Moves: %d\n", moves);
}
