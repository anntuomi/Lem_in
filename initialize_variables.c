#include "lemin.h"

void	initialize_variables(int path_count, t_routes ***used_routes,
t_routes ***tmp, t_var *var)
{
	int			i;
	t_routes	**new1;

	if (!(new1 = (t_routes **)malloc(sizeof(t_routes *) * path_count + 1)))
		handle_error();
	i = 0;
	while (i <= path_count)
	{
		new1[i] = NULL;
		i++;
	}
	*used_routes = NULL;
	*tmp = new1;
	var->least_moves = INT_MAX;
	var->moves = -1;
	var->new_path_count = path_count;
	var->current_path_count = -1;
	var->max_path_count = path_count;
}
