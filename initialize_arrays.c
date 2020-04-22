#include "lemin.h"

void			initialize_arrays(int path_count, t_routes ***used_routes,
t_routes ***most_uniques, int *most_paths)
{
	int			i;
	t_routes	**new1;
	t_routes	**new2;

	new1 = (t_routes **)malloc(sizeof(t_routes *) * path_count + 1);
	new2 = (t_routes **)malloc(sizeof(t_routes *) * path_count + 1);
	if (new1 == NULL || new2 == NULL)
		handle_error();
	i = 0;
	while (i <= path_count)
	{
		new1[i] = NULL;
		new2[i] = NULL;
		i++;
	}
	*used_routes = new1;
	*most_uniques = new2;
	*most_paths = -1;
}