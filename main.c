#include "lemin.h"

static void print_shortest_route(t_routes *shortest_route)
{
    t_route *route;

    printf("shortest route (%d rooms)\n", shortest_route->rooms);
    route = shortest_route->route;
    while (route)
    {
        printf("%s\n", route->room->name);
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
            printf("%s\n", route->room->name);
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

int         count_current_room_routes(t_room *current, t_routes *route_list)
{
    int count;
    t_routes *route;

    route = route_list;
    count = 0;
    while (route)
    {
        if (current->id == route->route->room->id)
            count++;
        route = route->next;
    }
    return (count);
}

void        move_ants(int amount, t_route **ant)
{
    int i;
    int moved;

    i = 0;
    moved = 0;
    while (i < amount)
    {
        if (ant[i]->room->type != END && (ant[i]->next->room->ant_count == 0 || ant[i]->next->room->type == END))
        {
            ant[i]->room->ant_count--;
            ant[i] = ant[i]->next;
            ant[i]->room->ant_count++;
            moved == 1 ? printf(" L%d-%s", i, ant[i]->room->name) : printf("L%d-%s", i, ant[i]->room->name);
            moved = 1;
        }
        i++;
    }
    printf("\n");
}

void        solve(t_route **ants, int amount, t_routes *shortest_start, t_routes *route_list, t_room *end)
{
    int i;
    t_routes *shortest;
    t_routes *route;
    int route_count;

    i = 0;
    shortest = shortest_start;
    route = route_list;
    while (end->ant_count != amount)
    {
        move_ants(amount, ants);
    }
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
    print_input(farm.amount, farm.rooms, farm.links);
    set_link(farm.rooms, farm.links);
    farm.routes = get_routes_to_end(farm.start);
    print_routes(farm.routes);
    farm.shortest_route = get_shortest_route(farm.routes);
    ants_to_start(farm.ants, farm.amount, farm.shortest_route);
    print_shortest_route(farm.shortest_route);
    farm.start->ant_count = farm.amount;
    solve(farm.ants, farm.amount, farm.shortest_route, farm.routes, farm.end);
    return (0);
}