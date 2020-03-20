#include "lemin.h"

static int      is_room(t_room *room, char *input)
{
    while (room)
    {
        if (ft_strequ(input, room->name))
            return (1);
        room = room->next;
    }
    return (0);
}

static int      count_words(char **array)
{
    int     words;

    words = 0;
    while (array[words])
        words++;
    return (words);
}

static t_link   *get_link(char *line, t_room *room)
{
    t_link  *link;
    char    **rooms;

    if (!(link = (t_link *)malloc(sizeof(t_link))) ||
    !(rooms = ft_strsplit(line, '-')) || count_words(rooms) != 2 ||
    !is_room(room, rooms[0]) || !is_room(room, rooms[1]))
        handle_error();
    link->room1 = rooms[0];
    link->room2 = rooms[1];
    free(rooms);
    link->next = NULL;
    return (link);
}

t_link          *get_links(char *line, t_room *room)
{
    t_link  *head;
    t_link  *link;
    char    **rooms;

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