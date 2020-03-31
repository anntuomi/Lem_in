#include "lemin.h"

static int		validate_room_input(char **input, t_room **head)
{
	int		size;
	t_room	*room;

	size = 0;
	while (input[size] != NULL)
		size++;
	if (size == 1 && ft_strchr(input[0], '-'))
	{
		ft_2ddel(input);
		return (NOTROOM);
	}
	if (size != 3 || (ft_isnum(input[1]) != 1 || ft_isnum(input[2]) != 1))
		handle_error();
	room = *head;
	while (room)
	{
		if (ft_strequ(input[0], room->name) == 1)
			handle_error();
		room = room->next;
	}
	return (ISROOM);
}

static t_room	*new_room(int room_type, char **input)
{
	t_room		*new;
	static int	id = 0;

	new = (t_room *)malloc(sizeof(t_room));
	if (new == NULL)
		handle_error();
	new->name = ft_strdup(input[0]);
	new->x = ft_atoi(input[1]);
	new->y = ft_atoi(input[2]);
	new->type = room_type;
	new->id = id++;
	new->ant_count = 0;
	new->paths = NULL;
	ft_2ddel(input);
	new->next = NULL;
	return (new);
}

int				determine_room_type(char *line)
{
	if (ft_strcmp(line, "##start") == 0)
		return (START);
	else if (ft_strcmp(line, "##end") == 0)
		return (END);
	else
		return (NORMAL);
}

static t_room	*save_room(t_room *room, t_room **head, char **input,
int *room_type)
{
	if (room == NULL)
	{
		room = new_room(*room_type, input);
		*head = room;
	}
	else
	{
		room->next = new_room(*room_type, input);
		room = room->next;
	}
	*room_type = NORMAL;
	return (room);
}

void			create_room_list(t_room **head, char **line, t_input **lines)
{
	int		room_type;
	char	**input;
	t_room	*room;

	room_type = NORMAL;
	room = NULL;
	while (get_next_line(0, line) == 1)
	{
		if (*line[0] == 'L')
			handle_error();
		if (*line[0] == '#')
			room_type = determine_room_type(*line);
		else
		{
			input = ft_strsplit(*line, ' ');
			if (validate_room_input(input, head) == NOTROOM)
				break ;
			room = save_room(room, head, input, &room_type);
		}
		set_input(lines, *line, 1);
	}
}
