/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_best_routes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuomine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/01 14:02:54 by atuomine          #+#    #+#             */
/*   Updated: 2020/06/01 14:02:56 by atuomine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

t_group			*save_group_copy(t_group *group, t_group *old)
{
	t_group		*new;

	if (old)
		delete_group(old);
	if (!(new = (t_group *)malloc(sizeof(t_group))))
		handle_error(0, "Malloc error");
	new->path_count = group->path_count;
	new->routes = group->routes;
	new->moves = group->moves;
	free(group);
	group = NULL;
	return (new);
}

t_group			*rebuild_routes(t_farm farm, int state)
{
	t_group		*group;
	t_branch	*branches;

	if (!(group = (t_group *)malloc(sizeof(t_group))))
		handle_error(0, "Malloc error");
	branches = get_branches_to_end(farm.start, state);
	group->path_count = count_routes(branches);
	group->routes = routes_to_array(group->path_count, branches);
	delete_branches(branches);
	if (group->path_count > 0)
	{
		group->moves = calculate_moves(group->routes, group->path_count,
		farm.ant_count);
	}
	else
		group->moves = -1;
	optimize_path_count(group, farm.ant_count, group->moves);
	return (group);
}

/*
** Resets room used statuses for the next iteration
*/

void			clear_used_status(t_room *rooms)
{
	t_room	*current;

	current = rooms;
	while (current)
	{
		current->used = 0;
		current->visited = 0;
		current = current->next;
	}
}

void			check_dfs(t_farm *farm, t_group **best, int reorder_by,
t_group *groups)
{
	t_room		**starting_rooms;
	int			group_size;
	int			orig_group_size;

	starting_rooms = count_group_size(farm->start, &orig_group_size);
	while (reorder_by < 6)
	{
		group_size = orig_group_size;
		groups = rebuild_routes_dfs(*farm, group_size, starting_rooms, 0);
		optimize_path_count(groups, farm->ant_count, groups->moves);
		if ((*best)->moves == -1 || (groups->moves != -1 && \
		groups->moves < (*best)->moves))
			*best = save_group_copy(groups, *best);
		else if (groups)
			delete_group(groups);
		if (reorder_by == 5)
			break ;
		starting_rooms = reorder(starting_rooms, &group_size, reorder_by, 0);
		reorder_by++;
		clear_used_status(farm->rooms);
	}
	if (starting_rooms)
		free(starting_rooms);
}

/*
** 1. Runs EK to find the shortest route to end, and marks the flows on paths
**    (1 towards end, -1 away from end).
** 2. Uses rebuild_routes to build a route. Rebuild_routes uses BFS to move
**    through paths that have flow of 1. Paths that have flow other than 1 are
**    ignored. As a result we get a group of routes that do not overlap with
**    each other. After the search we calculate how many moves we would get
**    if we used 1 of the selected routes, 2 of the selected routes etc. until
**    we get the optimal amount of routes we should use (the group of routes
**    that results in the least amount of moves);
**    We also do a dfs-style search in a similar way to see more combinations.
** 3. Repeats steps 1 and 2. until no new start-end connections can be made.
**    Note that EK is basically like reversed build_routes: It will go through
**    paths that have either flow UNUSED or -1. More info in
**    edmonds_karp_traverse.c
*/

void			find_best_routes(t_farm *farm)
{
	int			path_found;
	t_group		*groups;
	t_group		*best;

	groups = NULL;
	best = rebuild_routes(*farm, SIMPLE);
	path_found = 1;
	while (path_found)
	{
		clear_used_status(farm->rooms);
		if ((path_found = edmonds_karp_traverse(*farm)))
		{
			groups = rebuild_routes(*farm, FLOWS);
			if (best->moves == -1 || (groups->moves != -1 && \
			groups->moves < best->moves))
				best = save_group_copy(groups, best);
			else if (groups)
				delete_group(groups);
			check_dfs(farm, &best, 1, NULL);
		}
	}
	farm->route_count = best->path_count;
	farm->ordered = best->routes;
	free(best);
}
