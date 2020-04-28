#include "lemin.h"

static int			is_unique(t_routes **used_list, t_route *tested_head)
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

static t_routes		*find_next_unique(int i, t_routes **used_routes,
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

static t_routes		**save_routes(t_routes **temp, t_routes **used, \
t_variables *var)
{
	int i;

	i = 0;
	if (var->moves < var->least_moves)
	{
		//printf("Current path count: %d\n", var->current_path_count);
		while (temp[i] && i < var->current_path_count)
		{
			used[i] = temp[i];
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

static t_variables	update_lowest_moves(t_routes **temp_routes, \
t_variables var, int j, int amount)
{
	double tested_moves;

	//printf("J: %d ", j);
	if (j == var.orig_path_count)
		tested_moves = calculate_moves(temp_routes, j, amount);
	else if (temp_routes[j] != NULL)
		tested_moves = calculate_moves(temp_routes, j + 1, amount);
	if ((temp_routes[j] != NULL || j == var.orig_path_count) && \
		(var.moves == -1 || tested_moves < var.moves))
	{
		var.moves = tested_moves;
		var.current_path_count = j + 1;
	}
	return (var);
}

t_routes			**determine_used_routes(t_farm farm, int *path_count,
int i, int j)
{
	t_routes	**tmp_routes;
	t_routes	**used_routes;
	t_variables var;

	initialize_arrays(*path_count, &used_routes, &tmp_routes, &var);
	if (farm.ordered[0]->rooms == 2)
	{
		free(tmp_routes);
		*path_count = 1;
		used_routes[0] = farm.ordered[0];
		return (used_routes);
	}
	while ((farm.ordered[i]))
	{
		tmp_routes[0] = farm.ordered[i];
		var = update_lowest_moves(tmp_routes, var, 0, farm.amount);
		j = 1;
		while (j <= *path_count)
		{
			tmp_routes[j] = find_next_unique(0, tmp_routes, farm.ordered);
			var = update_lowest_moves(tmp_routes, var, j, farm.amount);
			if (tmp_routes[j] == NULL)
			{
				save_routes(tmp_routes, used_routes, &var);
				break ;
			}
			j++;
		}
		//if (j == *path_count)
			//save_routes(tmp_routes, used_routes, &var);
		i++;
	}
	*path_count = var.new_path_count;
	free(tmp_routes);
	//printf("Least moves: %f\n", var.least_moves);
	return (used_routes);
}
