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

static t_route	*get_route(t_fork *forks, int rooms, t_room *room, t_room *prev,
int flags)
{
	t_route		*route;
	static int	id = 0;

	if (!(route = (t_route *)malloc(sizeof(t_route))))
		handle_error(flags, "Malloc error");
	route->id = id++;
	route->forks = forks;
	route->rooms = rooms;
	route->room = room;
	route->prev = prev;
	route->fork = NULL;
	route->next = NULL;
	room->used = 1;
	return (route);
}

t_route			*get_fork_route(t_route *before_fork, t_room *room, int flags)
{
	t_route		*route;
	t_fork		*fork;

	route = get_route(NULL, before_fork->rooms + 1, room, before_fork->room,
	flags);
	route->branch = before_fork->branch;
	fork = before_fork->forks;
	while (fork)
	{
		set_fork(route, fork->from, fork->to, flags);
		fork = fork->next;
	}
	set_fork(route, route->prev, route->room, flags);
	return (route);
}

static t_branch	*get_branch(t_room *room, t_room *prev, int fork, int flags)
{
	t_branch	*branch;
	static int	id = 0;

	if (!(branch = (t_branch *)malloc(sizeof(t_branch))))
		handle_error(flags, "Malloc error");
	branch->id = id++;
	branch->route = get_route(NULL, 2, room, prev, flags);
	branch->route->branch = branch;
	if (fork)
		set_fork(branch->route, prev, room, flags);
	branch->array = NULL;
	branch->routes = 1;
	branch->next = NULL;
	room->connection = START;
	return (branch);
}

static t_branch	*get_branches(t_room *start, int flags)
{
	t_branch	*head;
	t_branch	*branch;
	t_path		*path;
	t_room		*room;
	int			fork;

	room = NULL;
	fork = 0;
	if (is_connected_to_end(start, &room, &fork))
		return (get_branch(room, start, fork, flags));
	path = start->paths;
	while (path->flow != 1)
	{
		path = path->next;
	}
	head = get_branch(path->room, start, fork, flags);
	branch = head;
	path = path->next;
	int branch_counter = 1;
	while (path)
	{
		if (path->flow == 1)
		{
			branch->next = get_branch(path->room, start, 1, flags);
			branch_counter++;
			branch = branch->next;
		}
		path = path->next;
	}
	//printf("Branch counter %d\n", branch_counter);
	return (head);
}

t_branch		*get_branches_to_end(t_room *start, int flags)
{
	t_branch	*branches;

	branches = get_branches(start, flags);
	set_branches(&branches, flags);
	return (branches);
}
