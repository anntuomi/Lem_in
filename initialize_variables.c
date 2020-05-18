#include "lemin.h"

void			initialize_variables(int path_count, t_routes ***used_routes,
t_routes ***tmp, t_var *var)
{
	t_routes	**new;
	int			i;

	if (!(new = (t_routes **)malloc(sizeof(t_routes *) * (path_count + 1))))
		handle_error();
	i = 0;
	while (i <= path_count)
	{
		new[i] = NULL;
		i++;
	}
	*used_routes = NULL;
	*tmp = new;
	var->least_moves = INT_MAX;
	var->moves = -1;
	var->current_path_count = -1;
	var->new_path_count = path_count;
	var->max_path_count = path_count;
}
