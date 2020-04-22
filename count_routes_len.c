#include "lemin.h"

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
