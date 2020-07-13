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

void			set_fork(t_route *route, t_room *from, t_room *to, int flags)
{
	t_fork		*fork;

	if (!(fork = (t_fork *)malloc(sizeof(t_fork))))
		handle_error(flags, "Malloc error");
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

static void		set_route(t_route *route, t_room *room, int fork, int flags)
{
	route->rooms++;
	route->prev_room = route->room;
	route->room = room;
	room->visited = 1;
	if (fork)
		set_fork(route, route->prev_room, route->room, flags);
}

static void		set_fork_routes(t_branch *branch, t_route **route,
t_room **room, int flags)
{
	t_route		*first;
	t_route		*tmp;
	t_path		*path;

	first = *route;
	path = first->room->paths;
	while (path)
	{
		if (!((t_room *)path->room)->visited && path->flow == 1)
		{
			if (!*room)
				*room = path->room;
			else
			{
				tmp = *route;
				*route = get_fork_route(first, path->room, flags);
				(*route)->prev = tmp;
				(*route)->next = tmp->next;
				if (tmp->next)
					tmp->next->prev = *route;
				tmp->next = *route;
				branch->routes++;
			}
		}
		path = path->next;
	}
}

static int		set_routes(t_branch **branch, t_route **route, int flags)
{
	t_route		*first;
	t_room		*room;
	int			fork;
	int			id;

	room = NULL;
	if ((first = *route) && first->room->type != END)
	{
		if (!is_connected_to_end(first->room, &room, &fork))
			set_fork_routes(*branch, route, &room, flags);
		if (room)
			set_route(first, room, fork, flags);
		else
		{
			del_route(branch, route);
			return (0);
		}
		if (room && room->type == END)
		{
			id = (*branch)->id;
			*route = (*branch)->route;
			while (*branch && (*branch)->id == id)
			{
				if ((*route)->room->type != END)
					del_route(branch, route);
				else if (!(*route = (*route)->next))
					*branch = (*branch)->next;
			}
			return (0);
		}
	}
	if (!(*route = (*route)->next))
		*branch = (*branch)->next;
	return ((room && room->type != END ? 1 : 0));
}

void			set_branches(t_branch **head, int flags)
{
	t_branch	*branch;
	t_route		*route;
	int			rooms_left;

	rooms_left = 0;
	branch = *head;
	while (branch)
	{
		route = branch->route;
		while (route)
		{
			if (set_routes(&branch, &route, flags) && !rooms_left)
				rooms_left = 1;
		}
		if (branch && !branch->prev)
			*head = branch;
	}
	if (rooms_left)
		set_branches(head, flags);
}
