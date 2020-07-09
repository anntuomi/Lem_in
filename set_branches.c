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
	room->used = 1;
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
			if (!((t_room *)path->room)->used && path->flow == 1)
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
		}
		path = path->next;
	}
}

static int		set_routes(t_branch **branch, t_branch **prev_branch,
t_route **route, t_route **prev_route)
{
	t_route		*first;
	t_room		*room;
	int			fork;
	int			id;

	room = NULL;
	if ((first = *route) && first->room->type != END)
	{
		fork = 0;
		if (!is_connected_to_end(first->room, &room, &fork))
			set_fork_routes(*branch, route, &room, first->room->paths);
		if (room)
			set_route(first, room, fork);
		else
		{
			del_route(branch, prev_branch, route, *prev_route);
			return (0);
		}
		if (room && room->type == END)
		{
			id = (*branch)->id;
			*prev_route = NULL;
			*route = (*branch)->route;
			while (*branch && (*branch)->id == id)
			{
				if ((*route)->room->type != END)
					del_route(branch, prev_branch, route, *prev_route);
				else
				{
					*prev_route = *route;
					if (!(*route = (*route)->next))
					{
						*prev_branch = *branch;
						*branch = (*branch)->next;
					}
				}
			}
			return (0);
		}
	}
	*prev_route = *route;
	if (!(*route = (*route)->next))
	{
		*prev_branch = *branch;
		*branch = (*branch)->next;
	}
	return ((room && room->type != END ? 1 : 0));
}

void			set_branches(t_branch **head)
{
	t_branch	*branch;
	t_branch	*prev_branch;
	t_route		*route;
	t_route		*prev_route;
	int			rooms_left;

	rooms_left = 0;
	prev_branch = NULL;
	branch = *head;
	while (branch)
	{
		prev_route = NULL;
		route = branch->route;
		while (route)
		{
			if (set_routes(&branch, &prev_branch, &route, &prev_route) &&
			!rooms_left)
				rooms_left = 1;
		}
		if (!prev_branch)
			*head = branch;
	}
	if (rooms_left)
		set_branches(head);
}
