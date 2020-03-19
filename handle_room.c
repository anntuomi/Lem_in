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
    if (size != 3 || (ft_isnum(input[1]) != 1 || ft_isnum(input[2]) != 1))
        handle_error();
    return (ISROOM);
}

void	save_room(t_room **rooms, int room_type, char **input)
{
    t_room *new;

    new = (t_room *)malloc(sizeof(t_room));
    if (new == NULL)
        exit(1);
    new->name = ft_strdup(input[0]);
    new->x = ft_atoi(input[1]);
    new->y = ft_atoi(input[2]);
    new->type = room_type;
    ft_2ddel(input);
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
    int room_type;
    char **input;

    room_type = NORMAL;
    *rooms = NULL;
    while (get_next_line(0, line))
    {
        if (*line)
        {
            if (*line[0] == '#')
                room_type = determine_room_type(*line);
            else
            {
                input = ft_strsplit(*line, ' ');
                if (validate_room_input(input) == NOTROOM)
                    break ;
                save_room(rooms, room_type, input);
                room_type = NORMAL;
            }
        }
        free(*line);
    }
}
