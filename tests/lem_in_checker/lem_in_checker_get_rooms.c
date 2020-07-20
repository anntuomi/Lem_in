/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_rooms.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetelin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/13 14:03:36 by jmetelin          #+#    #+#             */
/*   Updated: 2020/07/13 14:03:37 by jmetelin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in_checker.h"
#include <stdio.h>

static t_room	*get_room(int type, char **input)
{
	t_room		*room;
	static int	id = 0;

	if (!(room = (t_room *)malloc(sizeof(t_room))))
		handle_error("Malloc error");
	room->type = type;
	room->name = ft_strdup(input[0]);
	room->x = ft_atoi(input[1]);
	room->y = ft_atoi(input[2]);
	input = NULL;
	room->id = id++;
	room->ant_count = 0;
	room->paths = NULL;
	room->next = NULL;
	return (room);
}

static void		set_room(int *type, char **input, t_room **head, t_room **room)
{
	if (!*room)
	{
		*room = get_room(*type, input);
		*head = *room;
	}
	else
	{
		(*room)->next = get_room(*type, input);
		*room = (*room)->next;
	}
	*type = NORMAL;
	del_array(input);
}

static int		is_room(char **input, t_room *room)
{
	int		words;

	words = count_words(input);
	if (words != 3)
	{
		del_array(input);
		return (0);
	}
	if (!is_nbr(input[1]) || !is_nbr(input[2]))
		handle_error("A room is not defined properly");
	while (room)
	{
		if (ft_strequ(room->name, input[0]))
			handle_error("Multiple rooms with the same name");
		room = room->next;
	}
	return (1);
}

static int		get_type(char *line)
{
	if (ft_strequ(line, "##start"))
		return (START);
	else if (ft_strequ(line, "##end"))
		return (END);
	else
		return (NORMAL);
}

t_room			*get_rooms(char **line)
{
	t_room		*head;
	t_room		*room;
	int			type;
	char		**input;

	head = NULL;
	room = NULL;
	type = NORMAL;
	while (get_next_line(0, line) == 1)
	{
		if (*line[0] == 'L')
			handle_error("Room name starts with 'L'");
		if (*line[0] == '#')
			type = get_type(*line);
		else
		{
			if (!(input = ft_strsplit(*line, ' ')))
				handle_error("Malloc error");
			if (!is_room(input, head))
				break ;
			set_room(&type, input, &head, &room);
		}
		ft_strdel(line);
	}
	return (head);
}
