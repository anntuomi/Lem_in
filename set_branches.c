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
		handle_error(0, "Malloc error");
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
	room->visited = 1;
	if (fork)
		set_fork(route, route->prev, route->room);
}

static void		set_fork_routes(t_branch *branch, t_route **route,
t_room **room, int state)
{
	t_route		*first;
	t_route		*tmp;
	t_path		*path;

	first = *route;
	path = first->room->paths;
	while (path)
	{
		if (((t_room *)path->room)->connection != START &&
		!((t_room *)path->room)->visited &&
		(path->flow == 1 || state == SIMPLE))
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

static int		set_routes(t_branch_set *set,
t_route **route, t_route **prev, int state)
{
	t_route		*first;
	t_room		*room;
	int			fork;

	room = NULL;
	if ((first = *route) && first->room->type != END)
	{
		fork = 0;
		if (!is_connected_to_end(first->room, &room, &fork, state))
			set_fork_routes(set->branch, route, &room, state);
		if (room)
			set_route(first, room, fork);
		else
			return (del_route(&set->branch, &set->prev, route, *prev));
		if (room && room->type == END)
			return (handle_room_is_end(set, prev, route));
	}
	*prev = *route;
	if (!(*route = (*route)->next))
	{
		set->prev = set->branch;
		set->branch = (set->branch)->next;
	}
	return ((room && room->type != END ? 1 : 0));
}

void			set_branches(t_branch **head, int state)
{
	t_route			*route;
	t_route			*prev_route;
	int				rooms_left;
	t_branch_set	set;

	rooms_left = 0;
	set.prev = NULL;
	set.branch = *head;
	while (set.branch)
	{
		prev_route = NULL;
		route = set.branch->route;
		while (route)
		{
			if (set_routes(&set, &route, &prev_route, state) &&
			!rooms_left)
				rooms_left = 1;
		}
		if (!set.prev)
			*head = set.branch;
	}
	if (rooms_left)
		set_branches(head, state);
}
