#include "lemin.h"

int main(int argc, char **argv)
{
    t_room  *rooms;
    t_link  *link;
    char    *line;

    create_room_list(&rooms, &line);
    while (rooms != NULL)
    {
        printf("%s |x %d|y %d! type %d\n", rooms->name, rooms->x, rooms->y, rooms->type);
        rooms = rooms->next;
    }
    link = get_links(line);
    return (0);
}