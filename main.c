#include "lemin.h"

static void print_input(int amount, t_room *rooms, t_link *links)
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

static void find_edges(t_room *room, t_room **start, t_room **end)
{
    *start = NULL;
    *end = NULL;
    while (room)
    {
        if (room->type == START)
            *start = room;
        if (room->type == END)
            *end = room;
        room = room->next;
    }
    if (*start == NULL || *end == NULL)
        handle_error();
}

int         main(void)
{
    t_farm  farm;
    int     amount;
    char    *line;

    farm.ants = get_ants(&amount);
    create_room_list(&farm.rooms, &line);
    if (!(line))
        handle_error();
    find_edges(farm.rooms, &farm.start, &farm.end);
    ants_to_start(farm.ants, amount, farm.start);
    farm.links = get_links(line, farm.rooms);
    print_input(amount, farm.rooms, farm.links);
    set_link(farm.rooms, farm.links);
    printf("%d\n", find_end(farm.start, NULL));
    return (0);
}