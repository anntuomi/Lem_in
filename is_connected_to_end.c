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

void			del_route(t_branch **branch, t_route **route)
{
	t_branch	*next;
	t_fork		*fork;
	t_fork		*tmp;

	!(*route)->prev ? ((*branch)->route = (*route)->next) :
	((*route)->prev->next = (*route)->next);
	if ((*route)->next)
		((*route)->next->prev = (*route)->prev);
	fork = (*route)->forks;
	while (fork)
	{
		tmp = fork;
		fork = fork->next;
		free(tmp);
	}
	free(*route);
	*route = (!(*route)->prev ? (*branch)->route : (*route)->prev);
	if (!--(*branch)->routes)
	{
		next = (*branch)->next;
		if ((*branch)->prev)
			(*branch)->prev->next = next;
		if ((*branch)->next)
			(*branch)->next->prev = (*branch)->prev;
		free(*branch);
		*branch = next;
	}
	else if (!*route)
		*branch = (*branch)->next;
}

int				is_connected_to_end(t_room *room, t_room **end, int *fork)
{
	t_path		*path;
	t_room		*next;
	int			paths;

	*end = NULL;
	*fork = 0;
	paths = 0;
	path = room->paths;
	while (path)
	{
		next = path->room;
		if (path->flow == 1 && next->type == END)
			*end = next;
		paths++;
		path = path->next;
	}
	if ((room->type == START && paths > 1) || paths > 2)
		*fork = 1;
	return ((*end ? 1 : 0));
}
