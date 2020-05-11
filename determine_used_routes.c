#include "lemin.h"

static t_routes	**save_routes(t_routes **tmp, t_routes **used, t_var *var)
{
	int			i;

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

static t_var	update_lowest_moves(t_routes **tmp_routes, t_var var, int i,
int ant_count)
{
	int			tested_moves;

	if (i == var.orig_path_count)
		tested_moves = calculate_moves(tmp_routes, i, ant_count);
	else if (tmp_routes[i])
		tested_moves = calculate_moves(tmp_routes, i + 1, ant_count);
	if ((tmp_routes[i] || i == var.orig_path_count) &&
	(var.moves == -1 || tested_moves < var.moves))
	{
		var.moves = tested_moves;
		var.current_path_count = i + 1;
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

static t_routes	**update_used_routes(t_var *var_pointer, t_routes **tmp,
t_routes **used, t_farm farm)
{
	t_var		var;
	int			i;

	i = 1;
	var = *var_pointer;
	while (i <= var.orig_path_count)
	{
		tmp[i] = find_next_unique(0, tmp, farm.ordered);
		var = update_lowest_moves(tmp, var, i, farm.ant_count);
		if (!tmp[i])
		{
			save_routes(tmp, used, &var);
			break ;
		}
		i++;
	}
	*var_pointer = var;
	return (used);
}

static t_routes	**one_move_route(t_routes **ordered, int *needed_routes)
{
	t_routes	**used_routes;

	if (!(used_routes = (t_routes **)malloc(sizeof(t_routes *) * 2)))
		handle_error();
	used_routes[0] = ordered[0];
	used_routes[1] = NULL;
	*needed_routes = 1;
	return (used_routes);
}

t_routes		**determine_used_routes(t_farm *farm)
{
	t_routes	**used_routes;
	t_routes	**tmp_routes;
	t_var		var;
	int			i;

	i = 0;
	if ((*farm).ordered[0]->rooms == 2)
		return (one_move_route((*farm).ordered, &(*farm).needed_routes));
	initialize_arrays((*farm).needed_routes, &used_routes, &tmp_routes, &var);
	while ((*farm).ordered[i])
	{
		tmp_routes[0] = (*farm).ordered[i];
		var = update_lowest_moves(tmp_routes, var, 0, (*farm).ant_count);
		used_routes = update_used_routes(&var, tmp_routes, used_routes, *farm);
		i++;
	}
	(*farm).needed_routes = var.new_path_count;
	free(tmp_routes);
	return (used_routes);
}
