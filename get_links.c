#include "lemin.h"

static t_room	*get_room(t_room *room, char *input)
{
	while (room)
	{
		if (ft_strequ(input, room->name))
			return (room);
		room = room->next;
	}
	handle_error();
}

static int		count_words(char **array)
{
	int	words;

	words = 0;
	while (array[words])
		words++;
	return (words);
}

static t_link	*get_link(char *line, t_room *room)
{
	t_link	*link;
	char	**rooms;

	link = (t_link *)malloc(sizeof(t_link));
	if (!link || !(rooms = ft_strsplit(line, '-')) || count_words(rooms) != 2)
		handle_error();
	link->room1 = get_room(room, rooms[0]);
	link->room2 = get_room(room, rooms[1]);
	link->next = NULL;
	ft_2ddel(rooms);
	return (link);
}

t_link			*get_links(char *line, t_room *room)
{
	t_link	*head;
	t_link	*link;
	char	**rooms;

	head = get_link(line, room);
	free(line);
	link = head;
	while (get_next_line(0, &line) == 1)
	{
		if (line[0] != '#')
		{
			link->next = get_link(line, room);
			link = link->next;
		}
		free(line);
	}
	return (head);
}
