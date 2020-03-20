#include "lemin.h"

void    print_input(int amount, t_room *rooms, t_link *links)
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

t_path  *get_path(char *room1, t_link *link, t_room *rooms_head)
{
    t_path  *path;
    t_room  *room;
    char    *room2;

    if (!(path = (t_path *)malloc(sizeof(t_path))))
            handle_error();
    room2 = ft_strequ(room1, link->room1) ? link->room2 : link->room1;
    room = rooms_head;
    while (room && !ft_strequ(room->name, room2))
        room = room->next;
    path->content = room;
    path->next = NULL;
    return (path);
}

void    set_link(t_room *rooms_head, t_link *links_head)
{
    t_room  *room;
    t_link  *link;
    t_path  *head;
    t_path  *path;

    room = rooms_head;
    while (room)
    {
        if (!(path = (t_path *)malloc(sizeof(t_path))))
            handle_error();
        head->content = NULL;
        head->next = NULL;
        path = head;
        link = links_head;
        while (link)
        {
            if (ft_strequ(link->room1, room->name) ||
            ft_strequ(link->room2, room->name))
            {
                path->next = get_path(room->name, link, rooms_head);
                path = path->next;
            }
            link = link->next;
        }
        room->paths = head->next;
        room = room->next;
    }
    
}

int     main(void)
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
    return (0);
}