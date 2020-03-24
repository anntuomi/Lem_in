#include "lemin.h"

static void print_input(int amount, t_room *rooms, t_link *links)
{
    ft_printf("%d\n", amount);
    while (rooms)
    {
        if (rooms->type == START)
            ft_printf("##start\n");
        else if (rooms->type == END)
            ft_printf("##end\n");
        ft_printf("%s %d %d\n", rooms->name, rooms->x, rooms->y);
        rooms = rooms->next;
    }
    while (links)
    {
        ft_printf("%s-%s\n", links->room1->name, links->room2->name);
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
    printf("%d\n", find_end(farm.start));
    printf("%d!\n", farm.start->visited);
    return (0);
}