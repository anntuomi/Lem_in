/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_branches_to_end.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuomine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/01 14:02:54 by atuomine          #+#    #+#             */
/*   Updated: 2020/06/01 14:02:56 by atuomine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static void		free_route(t_route *route)
{
	t_fork	*fork;
	t_fork	*tmp;

	fork = route->forks;
	while (fork)
	{
		tmp = fork;
		fork = fork->next;
		free(tmp);
	}
	free(route);
}

void			delete_group(t_group *group)
{
	t_route **route;
	int		i;

	route = group->routes;
	i = 0;
	while (route[i])
	{
		free(route[i]);
		i++;
	}
	free(group->routes);
	free(group);
	group = NULL;
}

t_group			*rebuild_routes(t_farm farm)
{
	t_group		*group;
	t_branch	*branches;
	int			i;
	int			length;
	t_path		*current;
	
	group = (t_group *)malloc(sizeof(t_group));
	branches = get_branches_to_end(farm.start);
	group->path_count = count_routes(branches);
	group->routes = routes_to_array(group->path_count, branches);
	free(branches);
	if (group->path_count > 0)
		group->moves = calculate_moves(group->routes, group->path_count, farm.ant_count);
	else
		group->moves = -1;
	group->next = NULL;
	return (group);
}

// Resets room used statuses for the next iteration

void			clear_used_status(t_room *rooms)
{
	t_room *current;
	
	current = rooms;
	while (current)
	{
		current->used = 0;
		current->used2 = 0;
		current = current->next;
	}
}

// 1. Runs EK to find the shortest route to end, and marks
//    the flows on paths (1 towards end, -1 away from end)
// 2. Uses rebuild_routes to build a route. Rebuild_routes
//    uses BFS to move through paths that have flow of 1.
//    Paths that have flow other than 1 are ignored.
// 3. Repeats steps 1 and 2. until no new start-end connections
//    can be made. Note that EK is basically like reversed 
//    build_routes: It will go through paths that
//    have either flow UNUSED or -1. More info in
//    edmonds_karp_traverse.c

void	find_best_routes(t_farm *farm)
{
	int path_found;
	t_group *groups;
	t_group *best;

	path_found = edmonds_karp_traverse(*farm);
	if (path_found)
		best = rebuild_routes(*farm);
	while (path_found)
	{
		clear_used_status(farm->rooms);
		path_found = edmonds_karp_traverse(*farm);
		if (path_found)
		{
			groups = rebuild_routes(*farm);
			if (best->moves == -1 || (groups->moves != -1 && groups->moves < best->moves))
			{
				delete_group(best);
				best = groups;
			}
			else if (groups)
				delete_group(groups);
		}
	}
	farm->path_count = best->path_count;
	farm->route_count = best->path_count;
	farm->ordered = best->routes;
	free(best);
}