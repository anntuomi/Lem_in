#include "lemin.h"

static int		is_dup(int id, t_path *path)
{
	t_room		*room;

	while (path)
	{
		room = path->room;
		if (room->id == id)
			return (1);
		path = path->next;
	}
	return (0);
}

static void		set_link(t_room *room1, t_room *room2)
{
	t_path		*new;
	t_path		*path;

	if (!is_dup(room2->id, room1->paths))
	{
		if (!(new = (t_path *)malloc(sizeof(t_path))))
			handle_error();
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
	handle_error();
}

int				count_words(void **array)
{
	int			words;

	words = 0;
	while (array[words])
		words++;
	return (words);
}

void			set_links(char *line, t_room *room, t_input **input)
{
	char		**rooms;
	t_room		*room1;
	t_room		*room2;
	int			first;

	first = 1;
	while (first || get_next_line(0, &line) == 1)
	{
		if (line[0] != '#')
		{
			if (!(rooms = ft_strsplit(line, '-')) ||
			count_words((void **)rooms) != 2)
				handle_error();
			if (!ft_strequ(rooms[0], rooms[1]))
			{
				room1 = get_room(rooms[0], room);
				room2 = get_room(rooms[1], room);
				set_link(room1, room2);
				set_link(room2, room1);
				ft_2ddel(rooms);
			}
		}
		set_input(input, line, 0);
		first = 0;
	}
}
