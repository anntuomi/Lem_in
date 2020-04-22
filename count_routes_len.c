#include "lemin.h"

void			del_routes_linked_list_array(t_routes **routes)
{
	t_routes	*tmp;
	int			i;

	i = 0;
	while (routes[i])
	{
		while (routes[i])
		{
			tmp = routes[i];
			routes[i] = routes[i]->next;
			free(tmp);
		}
		i++;
	}
	free(routes);
}

int				*count_routes_len(t_routes **routes, int size)
{
	int			*len;
	int			i;

	if (!(len = (int *)malloc(sizeof(int) * 2)))
		handle_error();
	len[0] = 0;
	len[1] = 0;
	i = 0;
	while (i < size)
	{
		if (routes[i])
		{
			len[0]++;
			len[1] += routes[i]->rooms;
		}
		i++;
	}
	return (len);
}
