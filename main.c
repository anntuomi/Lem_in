#include "lemin.h"

void   print_input(t_room *rooms, t_link *links)
{
    while (rooms)
    {
        if (rooms->type == START)
            printf("##start\n");
        else if (rooms->type == END)
            printf("##end\n");
        printf("%s %d %d\n", rooms->name, rooms->x, rooms->y);
        rooms = rooms->next;
    }
    while (links)
    {
        printf("%s-%s\n", links->room1, links->room2);
        links = links->next;
    }
}
int main(int argc, char **argv)
{
    t_room  *rooms;
    t_link  *links;
    char    *line;

    create_room_list(&rooms, &line);
    if (!(line))
        handle_error();
    links = get_links(line, rooms);
    print_input(rooms, links);
    return (0);
}