#include "lemin.h"

static void print_shortest_route(t_routes *shortest_route)
{
    t_route *route;

    printf("shortest route (%d rooms)\n", shortest_route->rooms);
    route = shortest_route->route;
    while (route)
    {
        printf("%d. %s\n", route->index, route->room->name);
        route = route->next;
    }
}

static void print_routes(t_routes *routes)
{
    t_route *route;

    while (routes)
    {
        printf("route (%d rooms)\n", routes->rooms);
        route = routes->route;
        while (route)
        {
            printf("%d. %s\n", route->index, route->room->name);
            route = route->next;
        }
        routes = routes->next;
    }
}

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
        printf("%s-%s\n", links->room1->name, links->room2->name);
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
    char    *line;

    farm.ants = get_ants(&farm.amount);
    create_room_list(&farm.rooms, &line);
    if (!(line))
        handle_error();
    find_edges(farm.rooms, &farm.start, &farm.end);
    farm.links = get_links(line, farm.rooms);
    set_link(farm.rooms, farm.links);
    farm.routes = get_routes_to_end(farm.start);
    print_input(farm.amount, farm.rooms, farm.links);
    print_routes(farm.routes);
    farm.shortest_route = get_shortest_route(farm.routes);
    print_shortest_route(farm.shortest_route);
    ants_to_start(farm.ants, farm.amount, farm.shortest_route);
    farm.start->ant_count = farm.amount;
    solve(farm.ants, farm.amount, farm.routes, farm.end);
    return (0);
}