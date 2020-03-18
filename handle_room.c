#include "lemin.h"

int      validate_room_input(char **input)
{
    int size;

    size = 0;
    while (input[size] != NULL)
        size++;
    if (size == 1 && ft_strchr(input[0], '-'))
    {
        ft_2ddel(input);
        return (NOTROOM);
    }
    if (size != 3)
        exit(2);
    if (ft_isnum(input[1]) != 1 || ft_isnum(input[2]) != 1)
        exit(3);
    return (ISROOM);
}

t_room    *save_room(int room_type, char **input)
{
    t_room *room;

    room = (t_room *)malloc(sizeof(t_room));
    if (room == NULL)
        exit(1);
    room->name = ft_strdup(input[0]);
    room->x = ft_atoi(input[1]);
    room->y = ft_atoi(input[2]);
    room->type = room_type;
    room->next = NULL;
    ft_2ddel(input);
    return (room);
}

void	append_room(t_room **rooms, int room_type, char **input)
{
    t_room *new;

    new = save_room(room_type, input);
	new->next = *rooms;
	*rooms = new;
}

int     determine_room_type(char *line)
{
    if (ft_strcmp(line, "##start") == 0)
        return (START);
    else if (ft_strcmp(line, "##end") == 0)
        return (END);
    else
        return (NORMAL);
}

void    create_room_list(t_room **rooms, char **line)
{
    int ret;
    int room_type;
    int i;
    char **input;

    i = 0;
    ret = 1;
    room_type = NORMAL;
    while (ret != 0)
    {
        ret = get_next_line(0, line);
        if (*line)
        {
            if (*line[0] == '#')
                room_type = determine_room_type(*line);
            else
            {
                input = ft_strsplit(*line, ' ');
                if (validate_room_input(input) == NOTROOM)
                    break ;
                if (i == 0)
                    *rooms = save_room(room_type, input);
                else
                    append_room(rooms, room_type, input);
                room_type = NORMAL;
            }
        }
        free(*line);
        i++;
    }
}
