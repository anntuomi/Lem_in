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

/*static void		free_rooms(t_room *room)
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

static void		free_current_route(t_route *route)
{
	t_route *next_route;

	while (route)
	{
		next_route = route->next;
		free(route);
		route = next_route;
	}
}

static void		free_routes(t_routes **ordered)
{
	int i;

	i = 0;
	while (ordered[i])
	{
		free_current_route(ordered[i]->route);
		free(ordered[i]);
		i++;
	}
	free(ordered);
}

void			free_memory(t_farm farm)
{
	free_rooms(farm.rooms);
	free(farm.ants);
	free_routes(farm.ordered);
}*/
