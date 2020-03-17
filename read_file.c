#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "libft.h"
#include "get_next_line.h"

#define START 0
#define NORMAL 1
#define END 2

typedef struct	s_room
{
    char *name;
    int x;
    int y;
    int type;
    void *next;
}				t_room;

t_room    *save_room(char *line, int room_type)
{
    t_room *room;
    char **input;

    input = ft_strsplit(line, ' ');
    room = (t_room *)malloc(sizeof(t_room));
    room->name = ft_strdup(input[0]);
    room->x = ft_atoi(input[1]);
    room->y = ft_atoi(input[2]);
    room->type = room_type;
    room->next = NULL;
    return (room);
}

void	append_room(t_room **rooms, char *line, int room_type)
{
    t_room *new;

    new = save_room(line, room_type);
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

void    handle_input(void)
{
    int ret;
    int room_type;
    int i;
    char *line;
    t_room *rooms;

    i = 0;
    ret = 1;
    room_type = 1;
    while (ret != 0)
    {
        ret = get_next_line(0, &line);
        if (line)
        {
            if (line[0] == '#')
                room_type = determine_room_type(line);
            else
            {
                if (i == 0)
                    rooms = save_room(line, room_type);
                else
                    append_room(&rooms, line, room_type);
                room_type = NORMAL;
            }
        }
        free(line);
        i++;
    }
    while (rooms != NULL)
    {
        printf("%s |x %d|y %d! type %d\n", rooms->name, rooms->x, rooms->y, rooms->type);
        rooms = rooms->next;
    }
}

int main(int argc, char **argv)
{
    handle_input();
    return (0);
}