/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuomine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/01 14:04:15 by atuomine          #+#    #+#             */
/*   Updated: 2020/06/01 14:04:17 by atuomine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

int				calculate_moves(t_routes **routes, int path_count,
int ant_count)
{
	int			moves;
	int			rooms;
	int			i;

	rooms = 0;
	i = 0;
	while (i < path_count)
		rooms += routes[i++]->rooms - 2;
	moves = (ant_count + rooms) / path_count +
	((ant_count + rooms) % path_count ? 1 : 0);
	return (moves);
}

static void		assign_paths(t_route **ants, int ant_count, t_routes **routes,
int path_count)
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
	free(routes);
}

void			solve(t_farm farm)
{
	t_routes	**used_routes;
	char		*output;
	char		*line;
	char		*tmp;
	int			len;
	int			moves;

	used_routes = determine_used_routes(&farm);
	order_routes(used_routes);
	assign_paths(farm.ants, farm.ant_count, used_routes, farm.path_count);
	output = NULL;
	len = 0;
	moves = 0;
	while (farm.end->ant_count != farm.ant_count)
	{
		line = move_ants(farm.ants);
		tmp = ft_append(output, line, &len, '\n');
		if (output)
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
