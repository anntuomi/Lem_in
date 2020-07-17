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

static t_route	*get_route(t_fork *forks, int rooms, t_room *room, t_room *prev)
{
	t_route		*route;

	if (!(route = (t_route *)malloc(sizeof(t_route))))
		handle_error(0, "Malloc error");
	route->forks = forks;
	route->rooms = rooms;
	route->room = room;
	route->prev = prev;
	route->fork = NULL;
	route->next = NULL;
	room->visited = 1;
	return (route);
}

t_route			*get_fork_route(t_route *before_fork, t_room *room)
{
	t_route		*route;
	t_fork		*fork;

	route = get_route(NULL, before_fork->rooms + 1, room, before_fork->room);
	fork = before_fork->forks;
	while (fork)
	{
		set_fork(route, fork->from, fork->to);
		fork = fork->next;
	}
	set_fork(route, route->prev, route->room);
	return (route);
}

static t_branch	*get_branch(t_room *room, t_room *prev, int fork)
{
	t_branch	*branch;
	static int	id = 0;

	if (!(branch = (t_branch *)malloc(sizeof(t_branch))))
		handle_error(0, "Malloc error");
	branch->id = id++;
	branch->route = get_route(NULL, 2, room, prev);
	if (fork)
		set_fork(branch->route, prev, room);
	branch->routes = 1;
	branch->next = NULL;
	room->connection = START;
	return (branch);
}

/*
** State can be either SIMPLE for pathfinding without flows,
** or FLOWS for finding paths where flows are 1.
*/

static t_branch	*get_branches(t_room *start, int state)
{
	t_branch	*head;
	t_branch	*branch;
	t_path		*path;
	t_room		*room;
	int			fork;

	room = NULL;
	fork = 0;
	if (is_connected_to_end(start, &room, &fork, state))
		return (get_branch(room, start, fork));
	path = start->paths;
	while (path->flow != 1 && state == FLOWS)
		path = path->next;
	head = get_branch(path->room, start, fork);
	branch = head;
	while ((path = path->next))
	{
		if (path->flow == 1 || state == SIMPLE)
		{
			branch->next = get_branch(path->room, start, 1);
			branch = branch->next;
		}
	}
	start->visited = 1;
	return (head);
}

t_branch		*get_branches_to_end(t_room *start, int state)
{
	t_branch	*branches;

	branches = get_branches(start, state);
	set_branches(&branches, state);
	return (branches);
}
