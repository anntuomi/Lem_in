#include "lemin.h"

void			print_output(char **output, int *len)
{
	write(1, *output, *len);
	free(*output);
	*output = NULL;
	*len = 0;
}

static void		assign_paths(t_route **ants, t_routes **routes, int path_count,
int ant_count)
{
	int			moves;
	int			i;
	int			j;

	moves = calculate_moves(routes, path_count, ant_count);
	i = 0;
	j = 0;
	while (i < ant_count)
	{
		if (j == path_count || routes[j]->rooms - 1 > moves)
		{
			j = 0;
			moves--;
		}
		ants[i++] = routes[j++]->route;
	}
	ants[i] = NULL;
}

int				calculate_moves(t_routes **routes, int path_count,
int ant_count)
{
	int			moves;
	int			rooms;
	int			i;

	i = 0;
	rooms = 0;
	while (i < path_count)
		rooms += routes[i++]->rooms - 2;
	moves = ant_count + rooms;
	moves = !(moves % path_count) ? moves / path_count : moves / path_count + 1;
	return (moves);
}

void			solve(t_farm farm)
{
	t_routes	**used_routes;
	char		*output;
	char		*line;
	char		*tmp;
	int			len;
	int			moves;

	output = NULL;
	line = NULL;
	used_routes = determine_used_routes(farm, &farm.path_count);
	order_routes(used_routes);
	assign_paths(farm.ants, used_routes, farm.path_count, farm.ant_count);
	len = 0;
	moves = 0;
	while (farm.end->ant_count != farm.ant_count)
	{
		line = move_ants(farm.ants);
		tmp = ft_append(output, line, &len, '\n');
		free(output);
		output = tmp;
		free(line);
		if (len > 1000 || farm.end->ant_count == farm.ant_count)
			print_output(&output, &len);
		moves++;
	}
	char *result = ft_itoa(moves);
	write(1, result, strlen(result));
}
