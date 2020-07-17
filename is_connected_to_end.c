/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_connected_to_end.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuomine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/01 14:02:46 by atuomine          #+#    #+#             */
/*   Updated: 2020/06/01 14:02:48 by atuomine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

int				del_route(t_branch **branch, t_branch **prev_branch,
t_route **route, t_route *prev_route)
{
	t_branch	*next;

	!prev_route ? ((*branch)->route = (*route)->next) :
	(prev_route->next = (*route)->next);
	delete_forks((*route)->forks);
	free(*route);
	*route = (!prev_route ? (*branch)->route : prev_route->next);
	next = (!--(*branch)->routes || !*route ? (*branch)->next : *branch);
	if (!(*branch)->routes)
	{
		if (*prev_branch)
			(*prev_branch)->next = next;
		free(*branch);
	}
	else if (!*route)
		*prev_branch = *branch;
	*branch = next;
	return (0);
}

int				handle_room_is_end(t_branch_set *set, t_route **prev_route,
t_route **route)
{
	int			id;

	id = (set->branch)->id;
	*prev_route = NULL;
	*route = (set->branch)->route;
	while (set->branch && (set->branch)->id == id)
	{
		if ((*route)->room->type != END)
			del_route(&set->branch, &set->prev, route, *prev_route);
		else
		{
			*prev_route = *route;
			if (!(*route = (*route)->next))
			{
				set->prev = set->branch;
				set->branch = (set->branch)->next;
			}
		}
	}
	return (0);
}

int				is_connected_to_end(t_room *room, t_room **end, int *fork,
int state)
{
	t_path		*path;
	t_room		*next;
	int			paths;

	paths = 0;
	path = room->paths;
	while (path)
	{
		next = path->room;
		if ((path->flow == 1 || state == SIMPLE) && next->type == END)
			*end = next;
		paths++;
		path = path->next;
	}
	if ((room->type == START && paths > 1) || paths > 2)
		*fork = 1;
	return ((*end ? 1 : 0));
}
