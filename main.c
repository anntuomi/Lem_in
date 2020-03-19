#include "lemin.h"

void   print_input(int amount, t_room *rooms, t_link *links)
{
    printf("%d\n", amount);
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
    char    **ants;
    int     amount;
    t_room  *rooms;
    t_link  *links;
    char    *line;

    ants = get_ants(&amount);
    create_room_list(&rooms, &line);
    if (!(line))
        handle_error();
    add_start_room(ants, amount, rooms);
    links = get_links(line, rooms);
    print_input(amount, rooms, links);
    return (0);
}