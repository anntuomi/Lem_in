/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rebuild_routes_dfs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuomine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/01 14:02:46 by atuomine          #+#    #+#             */
/*   Updated: 2020/06/01 14:02:48 by atuomine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

t_fork			*create_fork(t_room *from, t_room *to)
{
	t_fork *fork;

	fork = (t_fork *)malloc(sizeof(t_fork));
	fork->from = from;
	fork->to = to;
	fork->next = NULL;
	return (fork);
}

static t_route	**create_dfs_routes(t_farm farm, t_room **starting_rooms,
int *group_size, int j)
{
	int		i;
	t_route **routes;
	int		length;

	routes = (t_route **)malloc(sizeof(t_route *) * *group_size + 1);
	i = 0;
	j = *group_size - 1;
	while (i < *group_size)
	{
		routes[i] = (t_route *)malloc(sizeof(t_route));
		length = 2;
		routes[i]->forks = save_dfs_forks(farm.start, starting_rooms[j],
		&length, farm.end);
		routes[i]->rooms = length;
		if (length == 0)
		{
			*group_size = *group_size - 1;
			free(routes[i]);
		}
		else
			i++;
		j--;
	}
	routes[i] = NULL;
	return (routes);
}

t_group			*rebuild_routes_dfs(t_farm farm, int orig_group_size,
t_room **starting_rooms, int j)
{
	t_group *group;
	int		group_size;
	t_route	**routes;
	int		i;
	t_path	*current;

	group = (t_group *)malloc(sizeof(t_group));
	group_size = orig_group_size;
	group->routes = create_dfs_routes(farm, starting_rooms, &group_size, j);
	group->path_count = group_size;
	if (group->path_count > 0)
		group->moves = calculate_moves(group->routes, group->path_count,
		farm.ant_count);
	else
		group->moves = -1;
	group->next = NULL;
	return (group);
}
