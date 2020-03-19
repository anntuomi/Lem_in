#include "lemin.h"

int main(int argc, char **argv)
{
    t_room  *rooms;
    t_link  *links;
    char    *line;

    create_room_list(&rooms, &line);
    if (!(line))
    handle_error();
    get_links(&links, line);
    while (rooms != NULL)
    {
        printf("%s |x %d|y %d! type %d\n", rooms->name, rooms->x, rooms->y, rooms->type);
        rooms = rooms->next;
    }
        while (links != NULL)
    {
        printf("%s - %s\n", links->room1, links->room2);
        links = links->next;
    }
    return (0);
}