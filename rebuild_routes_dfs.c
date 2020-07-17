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

t_room			**reorder(t_room **original, int *group_size, int reorder_by,
int offset)
{
	t_room	**new;
	int		i;
	int		picker;

	if (!(new = (t_room **)malloc(sizeof(t_room *) * (*group_size + 1))))
		handle_error(0, "Malloc error");
	i = 0;
	while (offset < reorder_by && i < *group_size)
	{
		picker = offset + reorder_by;
		while (picker < *group_size)
		{
			new[i] = original[picker];
			picker = picker + reorder_by;
			i++;
		}
		new[i] = original[offset];
		i++;
		offset++;
	}
	new[i] = NULL;
	*group_size = i;
	if (original)
		free(original);
	return (new);
}

t_fork			*create_fork(t_room *from, t_room *to)
{
	t_fork *fork;

	if (!(fork = (t_fork *)malloc(sizeof(t_fork))))
		handle_error(0, "Malloc error");
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

	if (!(routes = (t_route **)malloc(sizeof(t_route *) * (*group_size + 1))))
		handle_error(0, "Malloc error");
	i = 0;
	while (i < *group_size)
	{
		if (!(routes[i] = (t_route *)malloc(sizeof(t_route))))
			handle_error(0, "Malloc error");
		routes[i]->rooms = 2;
		routes[i]->forks = save_dfs_forks(farm.start, starting_rooms[j],
		&routes[i]->rooms, farm.end);
		if (routes[i]->rooms == 0)
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

	if (!(group = (t_group *)malloc(sizeof(t_group))))
		handle_error(0, "Malloc error");
	group_size = orig_group_size;
	j = orig_group_size - 1;
	group->routes = create_dfs_routes(farm, starting_rooms, &group_size, j);
	group->path_count = group_size;
	if (group->path_count > 0)
		group->moves = calculate_moves(group->routes, group->path_count,
		farm.ant_count);
	else
		group->moves = -1;
	return (group);
}
