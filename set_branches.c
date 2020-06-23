/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_branches.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuomine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/01 14:02:54 by atuomine          #+#    #+#             */
/*   Updated: 2020/06/01 14:02:56 by atuomine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void			set_fork(t_route *route, t_room *from, t_room *to)
{
	t_fork		*fork;

	if (!(fork = (t_fork *)malloc(sizeof(t_fork))))
		handle_error();
	fork->from = from;
	fork->to = to;
	fork->next = NULL;
	if (!route->forks)
	{
		route->forks = fork;
		route->fork = route->forks;
	}
	else
	{
		route->fork->next = fork;
		route->fork = route->fork->next;
	}
}

static void		set_route(t_route *route, t_room *room, int fork)
{
	route->rooms++;
	route->prev = route->room;
	route->room = room;
	if (fork)
		set_fork(route, route->prev, route->room);
}

static void		set_fork_routes(t_branch *branch, t_route **route,
t_room **room, t_path *path)
{
	t_route		*first;
	t_route		*tmp;

	first = *route;
	while (path)
	{
		if (is_unvisited(path->room, first->prev, first->forks))
		{
			if (!*room)
				*room = path->room;
			else
			{
				tmp = *route;
				*route = get_fork_route(first, path->room);
				(*route)->next = tmp->next;
				tmp->next = *route;
				branch->routes++;
			}
		}
		path = path->next;
	}
}

static int		set_routes(t_branch *branch, t_route **route, t_route **prev)
{
	t_route		*first;
	t_room		*room;
	int			fork;

	first = *route;
	room = NULL;
	if (first->room->type != END)
	{
		fork = 0;
		if (!is_connected_to_end(first->room, &room, &fork))
			set_fork_routes(branch, route, &room, first->room->paths);
		if (room)
			set_route(first, room, fork);
		else
		{
			del_route(branch, route, *prev);
			return (0);
		}
	}
	*prev = *route;
	*route = (*route)->next;
	return ((room && room->type != END ? 1 : 0));
}

void			set_branches(t_branch *head)
{
	t_branch	*branch;
	t_route		*route;
	t_route		*prev;
	int			rooms_left;

	rooms_left = 0;
	branch = head;
	while (branch)
	{
		prev = NULL;
		route = branch->route;
		while (route)
		{
			if (set_routes(branch, &route, &prev) && !rooms_left)
				rooms_left = 1;
		}
		branch = branch->next;
	}
	if (rooms_left)
		set_branches(head);
}
