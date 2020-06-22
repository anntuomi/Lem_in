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
		handle_error();
	route->forks = forks;
	route->rooms = rooms;
	route->room = room;
	route->prev = prev;
	route->fork = NULL;
	route->next = NULL;
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

	if (!(branch = (t_branch *)malloc(sizeof(t_branch))))
		handle_error();
	branch->route = get_route(NULL, 2, room, prev);
	if (fork)
		set_fork(branch->route, prev, room);
	branch->routes = 1;
	branch->next = NULL;
	room->start_connection = 1;
	return (branch);
}

static t_branch	*get_branches(t_room *start)
{
	t_branch	*head;
	t_branch	*branch;
	t_path		*path;

	head = NULL;
	path = start->paths;
	while (path)
	{
		if (!head)
		{
			head = get_branch(path->room, start, (path->next ? 1 : 0));
			branch = head;
		}
		else
		{
			branch->next = get_branch(path->room, start, 1);
			branch = branch->next;
		}
		path = path->next;
	}
	if (!head)
		handle_error();
	return (head);
}

t_branch		*get_branches_to_end(t_room *start)
{
	t_branch	*branches;

	branches = get_branches(start);
	set_branches(branches);
	del_empty_branches(&branches);
	return (branches);
}
