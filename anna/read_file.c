#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "libft.h"
#include "get_next_line.h"

typedef struct	s_room
{
    char *name;
    int x;
    int y;
    void *next;
}				t_room;

//What is room name has spaces?
t_room    *save_room(char *line)
{
    t_room *room;
    char **input;

    input = ft_strsplit(line, ' ');
    room = (t_room *)malloc(sizeof(t_room));
    room->name = ft_strdup(input[0]);
    room->x = ft_atoi(input[1]);
    room->y = ft_atoi(input[2]);
    room->next = NULL;
    return (room);
}

void	append_room(t_room **rooms, char *line)
{
    t_room *new;

    new = save_room(line);
	new->next = *rooms;
	*rooms = new;
}

void    handle_input(void)
{
    int ret;
    int fd;
    int i;
    char *line;
    t_room *rooms;

    i = 0;
    ret = 1;
    while (ret != 0)
    {
        ret = get_next_line(0, &line);
        if (line)
        {
            if (i == 0)
                rooms = save_room(line);
            else
            {
                append_room(&rooms, line);
            }
            
        }
        free(line);
        i++;
    }
    while (rooms != NULL)
    {
        printf("%s |x %d|y %d!\n", rooms->name, rooms->x, rooms->y);
        rooms = rooms->next;
    }
}

int main(int argc, char **argv)
{
    handle_input();
    return (0);
}