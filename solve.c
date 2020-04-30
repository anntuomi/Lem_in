#include "lemin.h"

static void	assign_paths(t_route **ant, t_routes **ordered,
int path_count, int amount)
{
	int			rooms;
	int			moves;
	int			i1;
	int			i2;

	i1 = 0;
	rooms = 0;
	while (i1 < path_count)
		rooms += ordered[i1++]->rooms - 2;
	moves = amount + rooms;
	moves = !(moves % path_count) ? moves / path_count : moves / path_count + 1;
	i1 = 0;
	i2 = 0;
	while (i1 < amount)
	{
		if (i2 == path_count || ordered[i2]->rooms - 1 > moves)
		{
			i2 = 0;
			moves--;
		}
		ant[i1++] = ordered[i2++]->route;
	}
	ant[i1] = NULL;
}

static void	move_ants(t_farm farm)
{
	int			first;
	int			i;

	first = 1;
	i = 0;
	while (farm.ants[i])
	{
		if (farm.ants[i]->room->type != END &&
		(!farm.ants[i]->next->room->ant_count
		|| farm.ants[i]->next->room->type == END))
		{
			farm.ants[i]->room->ant_count--;
			farm.ants[i] = farm.ants[i]->next;
			farm.ants[i]->room->ant_count++;
			if (!first)
				printf(" ");
			else
				first = 0;
			printf("L%d-%s", i + 1, farm.ants[i]->room->name);
		}
		i++;
	}
	printf("\n");
}

int			find_longest_route(t_routes **used, int path_count)
{
	int i;
	int longest_route;

	i = 0;
	longest_route = used[0]->rooms - 2;
	while (i < path_count)
	{
		if (used[i]->rooms - 2 > longest_route)
			longest_route = used[i]->rooms - 2;
		i++;
	}
	return (longest_route);
}

int			calculate_moves(t_routes **used, int path_count, int ant_amount)
{
	int			moves;
	int			rooms;
	int			i;

	i = 0;
	rooms = 0;
	while (i < path_count)
		rooms += used[i++]->rooms - 2;
	moves = ant_amount + rooms;
	moves = !(moves % path_count) ? moves / path_count : moves / path_count + 1;
	return (moves);
}

/*int				adjust_to_ant_amount(int path_count, int ant_amount,
t_routes **ordered, t_routes **used)
{
	int			moves;
	int			new_path_count;
	int			tested_moves;

	if (ordered[0]->rooms == 2)
		return (-1);
	new_path_count = path_count;
	moves = calculate_moves(used, path_count, ant_amount);
	while (path_count > 0)
	{
		if ((tested_moves = calculate_moves(used, path_count, \
		ant_amount)) < moves)
		{
			moves = tested_moves;
			new_path_count = path_count;
		}
		//printf("Path count: %d Moves: %f ", path_count, tested_moves);
		path_count--;
	}
	if (calculate_moves(ordered, 1, ant_amount) < moves)
		return (-1);
	return (new_path_count);
}*/

void		solve(t_farm farm, t_routes **ordered, int path_count)
{
	t_routes	**used_routes;
	int			moves;

	used_routes = determine_used_routes(farm, &path_count);
	order_routes(used_routes);
	print_unique_routes(used_routes);
	printf("\n");
	assign_paths(farm.ants, used_routes, path_count, farm.amount);
	moves = 0;
	while (farm.end->ant_count != farm.amount)
	{
		move_ants(farm);
		moves++;
	}
	printf("Moves: %d\n", moves);
}
