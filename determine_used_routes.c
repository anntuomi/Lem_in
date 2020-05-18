#include "lemin.h"

static t_routes	**save_routes(t_routes **tmp, t_routes **used, t_var *var)
{
	int			i;

	if (used)
		free(used);
	if (!(used = (t_routes **)malloc(sizeof(t_routes *) *
	(var->current_path_count + 1))))
		handle_error();
	i = 0;
	while (tmp[i] && i < var->current_path_count)
	{
		used[i] = tmp[i];
		i++;
	}
	used[i] = NULL;
	var->new_path_count = var->current_path_count;
	if (var->moves != -1)
		var->least_moves = var->moves;
	return (used);
}

static void		count_combination_moves(t_var *var, t_routes **tmp_routes,
int i, int ant_count)
{
	int			tested_moves;

	if (i == var->max_path_count)
		tested_moves = calculate_moves(tmp_routes, i, ant_count);
	else if (tmp_routes[i])
		tested_moves = calculate_moves(tmp_routes, i + 1, ant_count);
	if ((tmp_routes[i] || i == var->max_path_count) &&
	(var->moves == -1 || tested_moves < var->moves))
	{
		var->moves = tested_moves;
		var->current_path_count = i + 1;
	}
}

/*
** Finds a combination of unique routes and saves it to tmp.
** Moves are counted after every new added path.
** If a combination with moves less than the previous
** least moves combination is found, **used is updated with the
** new lowest move combination.
*/

static void		check_for_lowest_move_combination(t_var *var,
t_routes ***used, t_routes **tmp, t_farm farm)
{
	int			i;

	count_combination_moves(var, tmp, 0, farm.ant_count);
	i = 1;
	while (i <= var->max_path_count)
	{
		tmp[i] = find_next_unique(farm.ordered, tmp);
		count_combination_moves(var, tmp, i, farm.ant_count);
		if (!tmp[i])
		{
			if (var->moves < var->least_moves)
				*used = save_routes(tmp, *used, var);
			var->moves = -1;
			break ;
		}
		i++;
	}
}

static t_routes	**one_move_route(t_routes **ordered, int *path_count)
{
	t_routes	**used_routes;

	if (!(used_routes = (t_routes **)malloc(sizeof(t_routes *) * 2)))
		handle_error();
	used_routes[0] = ordered[0];
	used_routes[1] = NULL;
	*path_count = 1;
	return (used_routes);
}

t_routes		**determine_used_routes(t_farm *farm)
{
	t_routes	**used_routes;
	t_routes	**tmp_routes;
	t_var		var;
	int			i;

	if (farm->ordered[0]->rooms == 2)
		return (one_move_route(farm->ordered, &farm->path_count));
	initialize_variables(farm->path_count, &used_routes, &tmp_routes, &var);
	i = 0;
	while (farm->ordered[i])
	{
		tmp_routes[0] = farm->ordered[i];
		check_for_lowest_move_combination(&var, &used_routes, tmp_routes,
		*farm);
		i++;
	}
	farm->path_count = var.new_path_count;
	free(tmp_routes);
	return (used_routes);
}
