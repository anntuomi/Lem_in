#include "lemin.h"

int main(int argc, char **argv)
{
    t_room *rooms;
    char *line;

    create_room_list(&rooms, &line);
    while (rooms != NULL)
    {
        printf("%s |x %d|y %d! type %d\n", rooms->name, rooms->x, rooms->y, rooms->type);
        rooms = rooms->next;
    }
    printf("%s", line);
    return (0);
}