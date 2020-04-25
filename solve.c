#include "lemin.h"

static t_route	**assign_paths(t_route **ant, t_routes **ordered,
int path_count, int amount)
{
	int			i;

	i = 0;
	while (i < amount)
	{
		//printf("Ant number %d (Path_count: %d) Mod: %d\n", i, path_count,
		//(i) % path_count);
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

double			calculate_moves(t_routes **used, int path_count, int ant_amount)
{
	double	float_result;
	double	last_result;
	double	remaining_moves;

	if (path_count > ant_amount)
		path_count = ant_amount;
	//printf("Most rooms: %d extra ants: %d\n", used[path_count - 1]->rooms - 2, ant_amount % path_count);
	float_result = ((double)used[path_count - 1]->rooms - 2) + \
	((double)ant_amount / (double)path_count);
	if (ant_amount % path_count != 0)
	{
		remaining_moves = (double)used[path_count - 1]->rooms - 2;
		path_count = ant_amount % path_count;
		last_result = ((double)used[path_count - 1]->rooms - 2.0) + 1;
		if (remaining_moves < last_result)
			float_result = float_result + 1.0;
	}
	return (float_result);
}

int				adjust_to_ant_amount(int path_count, int ant_amount,
t_routes **ordered, t_routes **used)
{
	double	moves;
	int		new_path_count;
	double	tested_moves;

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
}

void			solve(t_farm farm, t_routes **ordered, int path_count)
{
	t_routes	**used_routes;
	int			moves;

	used_routes = determine_used_routes(farm, &path_count, 0, 0);
	assign_paths(farm.ants, used_routes, path_count, farm.amount);
	moves = 0;
	while (farm.end->ant_count != farm.amount)
	{
		move_ants(farm.amount, farm.ants, farm.routes);
		moves++;
	}
	printf("Moves: %d\n", moves);
}
