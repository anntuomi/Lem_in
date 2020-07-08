/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuomine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/01 14:02:31 by atuomine          #+#    #+#             */
/*   Updated: 2020/06/01 14:02:33 by atuomine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void			ft_delete(char **array)
{
	int i;

	i = 0;
	while (array[i] != NULL)
	{
		ft_strdel(&array[i]);
		i++;
	}
	free(array);
	array = NULL;
}

static void		free_rooms(t_room *room)
{
	t_room *next_room;
	t_path *next_path;

	while (room)
	{
		next_room = room->next;
		free(room->name);
		while (room->paths)
		{
			next_path = room->paths->next;
			free(room->paths);
			room->paths = next_path;
		}
		free(room);
		room = next_room;
	}
}

static void		free_route(t_route *route)
{
	t_fork	*fork;
	t_fork	*tmp;

	fork = route->forks;
	while (fork)
	{
		tmp = fork;
		fork = fork->next;
		free(tmp);
	}
	free(route);
}

static void		free_branches(t_branch *branch)
{
	int i;

	i = 0;
	while (branch)
	{
		free_route(branch->route);
		free(branch);
		branch = branch->next;
	}
}

void			free_memory(t_farm farm)
{
	free_rooms(farm.rooms);
	free(farm.ants);
	free_branches(farm.branches);
}
