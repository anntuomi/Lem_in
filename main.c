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

void    find_edges(t_room *room, t_room **start, t_room **end)
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

void    set_link(t_room **rooms_head, t_link **links_head)
{
    t_room *room;
    t_link *link;
    t_room *tmp;

    link = *links_head;
    while (link)
    {
        room = *rooms_head;
        while (room)
        {
            if (ft_strequ(room->name, link->room1) == 1)
            {
                tmp = *rooms_head;
                printf("Room1 found, name: %s\n", room->name);
                while (tmp && ft_strequ(tmp->name, link->room2) != 1)
                    tmp = tmp->next;
                printf("Room2 found, name: %s\n", tmp->name);
            }
            room = room->next;
        }
        link = link->next;
    }
}

int main(int argc, char **argv)
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
    set_link(&farm.rooms, &farm.links);
    return (0);
}