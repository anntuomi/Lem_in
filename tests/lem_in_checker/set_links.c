/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_links.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetelin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/13 14:03:36 by jmetelin          #+#    #+#             */
/*   Updated: 2020/07/13 14:03:37 by jmetelin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in_checker.h"

static int		is_dup(int id, t_path *path)
{
	while (path)
	{
		if (((t_room *)path->room)->id == id)
			return (1);
		path = path->next;
	}
	return (0);
}

static void		set_link(t_room *room1, t_room *room2)
{
	t_path	*new;
	t_path	*path;

	if (!is_dup(room2->id, room1->paths))
	{
		if (!(new = (t_path *)malloc(sizeof(t_path))))
			handle_error("Malloc error");
		new->room = room2;
		new->next = NULL;
		if (!room1->paths)
			room1->paths = new;
		else
		{
			path = room1->paths;
			while (path->next)
				path = path->next;
			path->next = new;
		}
	}
}

static t_room	*get_room(char *input, t_room *room)
{
	while (room)
	{
		if (ft_strequ(room->name, input))
			return (room);
		room = room->next;
	}
	handle_error("A link includes an undefined room");
	return (NULL);
}

static int		is_link(char *line, char ***rooms)
{
	if (line[0] == '\0')
	{
		ft_strdel(&line);
		return (0);
	}
	else if (!(*rooms = ft_strsplit(line, '-')) || count_words(*rooms) != 2)
		handle_error("Malloc error or a link is not defined properly");
	ft_strdel(&line);
	return (1);
}

void			set_links(char *line, t_room *room)
{
	char	**rooms;
	t_room	*room1;
	t_room	*room2;
	int		first;

	first = 1;
	while (first || get_next_line(0, &line) == 1)
	{
		if (line[0] != '#')
		{
			if (!is_link(line, &rooms))
				break ;
			if (!ft_strequ(rooms[0], rooms[1]))
			{
				room1 = get_room(rooms[0], room);
				room2 = get_room(rooms[1], room);
				set_link(room1, room2);
				set_link(room2, room1);
			}
			del_array(rooms);
		}
		if (first)
			first = 0;
	}
}
