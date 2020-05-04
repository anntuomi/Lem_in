#include "lemin.h"

static void		assign_paths(t_route **ants, t_routes **routes, int path_count,
int amount)
{
	int			moves;
	int			i1;
	int			i2;

	moves = calculate_moves(routes, path_count, amount);
	i1 = 0;
	i2 = 0;
	while (i1 < amount)
	{
		if (i2 == path_count || routes[i2]->rooms - 1 > moves)
		{
			i2 = 0;
			moves--;
		}
		ants[i1++] = routes[i2++]->route;
	}
	ants[i1] = NULL;
}

/*int			find_longest_route(t_routes **used, int path_count)
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
}*/

int				calculate_moves(t_routes **routes, int path_count,
int ant_amount)
{
	int			moves;
	int			rooms;
	int			i;

	i = 0;
	rooms = 0;
	while (i < path_count)
		rooms += routes[i++]->rooms - 2;
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

void			print_output(char **output, int *len)
{
	write(1, *output, *len);
	free(*output);
	*output = NULL;
	*len = 0;
}

void			solve(t_farm farm, t_routes **ordered, int path_count)
{
	t_routes	**used_routes;
	//int			moves;
	char		*output;
	char		*line;
	char		*tmp;
	int			len;

	output = NULL;
	line = NULL;
	used_routes = determine_used_routes(farm, &path_count);
	order_routes(used_routes);
	//print_unique_routes(used_routes);
	//printf("\n");
	assign_paths(farm.ants, used_routes, path_count, farm.amount);
	//moves = 0;
	len = 0;
	while (farm.end->ant_count != farm.amount)
	{
		line = move_ants(farm.ants);
		tmp = ft_strjoin_new(output, line, &len, '\n');
		free(output);
		output = tmp;
		//moves++;
		free(line);
		if (len > 1000 || farm.end->ant_count == farm.amount)
			print_output(&output, &len);
	}
	//printf("Moves: %d\n", moves);
}
