/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_ants.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuomine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/01 14:03:43 by atuomine          #+#    #+#             */
/*   Updated: 2020/06/01 14:03:46 by atuomine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static void		take_step(t_ant *ant, t_room *room, int is_fork)
{
	ant->room->ant_count--;
	ant->prev = ant->room;
	ant->room = room;
	ant->room->ant_count++;
	if (is_fork)
		ant->fork = ant->fork->next;
}

static t_block	*make_block(char *name, int i)
{
	t_block		*block;

	if (!(block = (t_block *)malloc(sizeof(t_block))))
		handle_error(0, "Malloc error");
	if (!(block->ant_nbr = ft_itoa(i + 1)))
		handle_error(0, "Malloc error");
	block->ant_room_name = name;
	return (block);
}

static t_room	*set_up(t_ant **ants, int i, int *is_fork)
{
	t_room		*room;

	if (!ants[i]->fork || ants[i]->room->id != ants[i]->fork->from->id)
	{
		if (!ants[i]->prev ||
		((t_room *)ants[i]->room->paths->room)->id != ants[i]->prev->id)
			room = ants[i]->room->paths->room;
		else
			room = ants[i]->room->paths->next->room;
	}
	else
	{
		room = ants[i]->fork->to;
		*is_fork = 1;
	}
	return (room);
}

char			*move_ants(t_ant **ants)
{
	int			first;
	int			i;
	char		*line;
	t_room		*room;
	int			is_fork;

	first = 1;
	i = 0;
	line = NULL;
	while (ants[i])
	{
		is_fork = 0;
		if (ants[i]->room->type != END)
		{
			room = set_up(ants, i, &is_fork);
			if (!room->ant_count || room->type == END)
			{
				take_step(ants[i], room, is_fork);
				line = add_to_command_line(line,
				make_block(ants[i]->room->name, i), &first);
			}
		}
		i++;
	}
	return (line);
}
