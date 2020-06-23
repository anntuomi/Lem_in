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

void			*del_empty_branches(t_branch **head)
{
	t_branch	*branch;
	t_branch	*prev;

	while (*head && !(*head)->routes)
	{
		branch = *head;
		*head = (*head)->next;
		free(branch);
	}
	if (!(prev = *head))
		handle_error();
	branch = prev->next;
	while (branch)
	{
		if (!branch->routes)
		{
			prev->next = branch->next;
			free(branch);
		}
		else
			prev = prev->next;
		branch = prev->next;
	}
}

void			del_route(t_branch *branch, t_route **route, t_route *prev)
{
	t_route		*next;
	t_fork		*fork;
	t_fork		*tmp;

	next = (*route)->next;
	if (!prev)
		branch->route = next;
	else
		prev->next = next;
	fork = (*route)->forks;
	while (fork)
	{
		tmp = fork;
		fork = fork->next;
		free(tmp);
	}
	free(*route);
	*route = next;
	branch->routes--;
}

int				is_unvisited(t_room *room, t_room *prev, t_fork *fork)
{
	if (!room->start_connection && room->id != prev->id)
	{
		while (fork)
		{
			if (fork->from->id == room->id)
				return (0);
			fork = fork->next;
		}
		return (1);
	}
	return (0);
}

int				is_connected_to_end(t_room *room, t_room **end, int *fork)
{
	t_path		*path;
	t_room		*next;
	int			paths;

	paths = 0;
	path = room->paths;
	while (path)
	{
		next = path->room;
		if (next->type == END)
			*end = next;
		paths++;
		path = path->next;
	}
	if ((room->type == START && paths > 1) || paths > 2)
		*fork = 1;
	return ((*end ? 1 : 0));
}
