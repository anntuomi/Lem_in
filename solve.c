#include "lemin.h"

static t_route *check_for_shorter(t_route *ant, t_routes *route, int len)
{
    while (route)
    {
        while(route->route->room->id != ant->room->id)
            route->route = route->route->next;
        if (route->rooms - route->route->index < len && \
            route->route->next->room->ant_count == 0)
        {
            len = route->rooms - route->route->index;
            ant = route->route;
        }
        route = route->next;
    }
    return (ant);
}

static t_route *change_route(t_route *ant, t_routes *route_list)
{
    t_routes *route;
    int      len;

    route = route_list;
    while (route)
    {
        while(route->route->room->id != ant->room->id)
            route->route = route->route->next;
        if (route->route->next->room->ant_count == 0)
        {
            ant = route->route;
            len = route->rooms - route->route->index;
            route = route->next;
            ant = check_for_shorter(ant, route, len);               
            break ;
        }        
        route = route->next;
    }
    return (ant);
}

static void move_ants(int amount, t_route **ant, t_routes *route_list)
{
    int i;
    int moved;
    t_routes *route;
    int len;

    i = 0;
    moved = 0;
    while (i < amount)
    {
        if (ant[i]->room->type != END && (ant[i]->next->room->ant_count != 0 \
            && ant[i]->next->room->type != END))
            ant[i] = change_route(ant[i], route_list);
        if (ant[i]->room->type != END)
        {
            ant[i]->room->ant_count--;
            ant[i] = ant[i]->next;
            ant[i]->room->ant_count++;
            moved == 1 ? printf(" L%d-%s", i + 1, ant[i]->room->name) : \
            printf("L%d-%s", i + 1, ant[i]->room->name);
            moved = 1;
        }
        i++;
    }
    printf("\n");
}

void        solve(t_route **ants, int amount, t_routes *route_list, t_room *end)
{
    int i;
    int route_count;

    i = 0;
    while (end->ant_count != amount)
    {
        move_ants(amount, ants, route_list);
    }
}