#include "lemin.h"

static t_routes		**save_routes(t_routes **tmp, t_routes **used, \
t_var *var)
{
	int i;

	i = 0;
	if (var->moves < var->least_moves)
	{
		while (tmp[i] && i < var->current_path_count)
		{
			used[i] = tmp[i];
			i++;
		}
		while (i < var->orig_path_count)
		{
			used[i] = NULL;
			i++;
		}
		var->new_path_count = var->current_path_count;
		if (var->moves != -1)
			var->least_moves = var->moves;
	}
	var->moves = -1;
	return (used);
}

static t_var	update_lowest_moves(t_routes **tmp_routes, \
t_var var, int j, int ant_count)
{
	int tested_moves;

	if (j == var.orig_path_count)
		tested_moves = calculate_moves(tmp_routes, j, ant_count);
	else if (tmp_routes[j])
		tested_moves = calculate_moves(tmp_routes, j + 1, ant_count);
	if ((tmp_routes[j] || j == var.orig_path_count) && \
		(var.moves == -1 || tested_moves < var.moves))
	{
		var.moves = tested_moves;
		var.current_path_count = j + 1;
	}
	return (var);
}

/*
** Finds a combination of unique routes and saves it to tmp.
** Moves are counted after every new added path.
** If a combination with moves less than the previous
** least moves combination is found, **used is updated with the
** new lowest move combination.
*/

static t_routes		**update_used_routes(t_var *var_pointer, t_routes **tmp,
t_routes **used, t_farm farm)
{
	int			j;
	t_var		var;

	j = 1;
	var = *var_pointer;
	while (j <= var.orig_path_count)
	{
		tmp[j] = find_next_unique(0, tmp, farm.ordered);
		var = update_lowest_moves(tmp, var, j, farm.ant_count);
		if (!tmp[j])
		{
			save_routes(tmp, used, &var);
			break ;
		}
		j++;
	}
	*var_pointer = var;
	return (used);
}

static t_routes		**one_move_route(t_routes **ordered, int **path_count)
{
	t_routes **used_routes;

	if (!(used_routes = (t_routes **)malloc(sizeof(t_routes *) * 2)))
		handle_error();
	used_routes[0] = ordered[0];
	used_routes[1] = NULL;
	**path_count = 1;
	return (used_routes);
}

t_routes			**determine_used_routes(t_farm farm, int *path_count)
{
	t_routes	**tmp_routes;
	t_routes	**used_routes;
	t_var		var;
	int			i;

	i = 0;
	if (farm.ordered[0]->rooms == 2)
		return (one_move_route(farm.ordered, &path_count));
	initialize_arrays(*path_count, &used_routes, &tmp_routes, &var);
	while ((farm.ordered[i]))
	{
		tmp_routes[0] = farm.ordered[i];
		var = update_lowest_moves(tmp_routes, var, 0, farm.ant_count);
		used_routes = update_used_routes(&var, tmp_routes, used_routes, farm);
		i++;
	}
	*path_count = var.new_path_count;
	free(tmp_routes);
	return (used_routes);
}
