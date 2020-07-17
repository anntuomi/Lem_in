/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_group_size.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuomine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/01 14:02:54 by atuomine          #+#    #+#             */
/*   Updated: 2020/06/01 14:02:56 by atuomine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static t_room	**create_starting_room_array(t_room *start, int group_size)
{
	t_path	*current;
	t_room	**starting_rooms;
	int		i;

	starting_rooms = (t_room **)malloc(sizeof(t_room *) * group_size + 1);
	current = start->paths;
	i = 0;
	while (current)
	{
		if (current->flow == 1)
		{
			starting_rooms[i] = (t_room *)current->room;
			i++;
		}
		current = current->next;
	}
	starting_rooms[i] = NULL;
	return (starting_rooms);
}

t_room			**count_group_size(t_room *start, int *group_size)
{
	int		size;
	t_path	*current;
	t_room	**starting_rooms;
	int		i;

	size = 0;
	current = start->paths;
	while (current)
	{
		if (current->flow == 1)
			size++;
		current = current->next;
	}
	*group_size = size;
	if (size == 0)
		return (NULL);
	return (create_starting_room_array(start, *group_size));
}
