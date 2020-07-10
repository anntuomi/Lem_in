/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_room.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuomine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/01 14:03:04 by atuomine          #+#    #+#             */
/*   Updated: 2020/06/01 14:03:06 by atuomine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static t_room	*new_room(int room_type, char **input, t_room *prev, int flags)
{
	t_room		*new;
	static int	id = 0;
	long long	tmp;

	if (!(new = (t_room *)malloc(sizeof(t_room))) ||
	!(new->name = ft_strdup(input[0])) ||
	(tmp = ft_atoll(input[1])) < INT_MIN || tmp > INT_MAX)
		handle_error(flags, "Malloc error or coord_x is not an integer");
	new->x = (int)tmp;
	if ((tmp = ft_atoll(input[2])) < INT_MIN || tmp > INT_MAX)
		handle_error(flags, "Coord_y is not an integer");
	new->y = (int)tmp;
	new->type = room_type;
	new->id = id++;
	new->ant_count = 0;
	new->connection = NORMAL;
	new->used = 0;
	new->used2 = 0;
	new->branch = NULL;
	new->route = NULL;
	new->paths = NULL;
	new->prev = prev;
	new->next = NULL;
	ft_delete(input);
	return (new);
}

static t_room	*save_room(t_room *room, t_room **head, int *room_type,
char **input, int flags)
{
	if (!room)
	{
		room = new_room(*room_type, input, NULL, flags);
		*head = room;
	}
	else
	{
		room->next = new_room(*room_type, input, room, flags);
		room = room->next;
	}
	*room_type = NORMAL;
	return (room);
}

static int		validate_room_input(char **input, t_room **head, int flags)
{
	int			size;
	t_room		*room;

	size = 0;
	while (input[size])
		size++;
	if (size == 1 && ft_strchr(input[0], '-'))
	{
		ft_delete(input);
		return (NOT_ROOM);
	}
	if (size != 3 || !ft_isnum(input[1]) || !ft_isnum(input[2]))
		handle_error(flags, "A room is not defined by: name coord_x coord_y");
	room = *head;
	while (room)
	{
		if (ft_strequ(input[0], room->name))
			handle_error(flags, "Multiple rooms with the same name");
		room = room->next;
	}
	return (IS_ROOM);
}

int				determine_room_type(char *line)
{
	if (ft_strequ(line, "##start"))
		return (START);
	else if (ft_strequ(line, "##end"))
		return (END);
	else
		return (NORMAL);
}

void			create_room_list(t_room **head, char **line, t_input **lines,
int flags)
{
	int			room_type;
	char		**input;
	t_room		*room;

	room_type = NORMAL;
	room = NULL;
	while (get_next_line(0, line) == 1)
	{
		if (*line[0] == 'L')
			handle_error(flags, "Room name starts with 'L'");
		if (*line[0] == '#')
			room_type = determine_room_type(*line);
		else
		{
			if (!(input = ft_strsplit(*line, ' ')))
				handle_error(flags, "Malloc error");
			if (validate_room_input(input, head, flags) == NOT_ROOM)
				break ;
			room = save_room(room, head, &room_type, input, flags);
		}
		set_input(lines, *line, 1, flags);
	}
}
