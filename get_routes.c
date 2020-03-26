#include "lemin.h"

int         is_unvisited(t_room *room, t_route *route)
{
    while (route)
    {
        if (route->room->id == room->id)
            return (0);
        route = route->next;
    }
    return (1);
}

int         count_unvisited(t_path *path, t_route *route)
{
    int     unvisited;

    unvisited = 0;
    while (path)
    {
        if (is_unvisited(path->content, route))
            unvisited++;
        path = path->next;
    }
    return (unvisited);
}

t_route     *get_route(t_room *room)
{
    t_route     *route;

    if (!(route = (t_route *)malloc(sizeof(t_route))))
        handle_error();
    route->room = room;
    route->next = NULL;
    return (route);
}

void        *set_route_before_fork(t_routes *routes, t_routes *before_fork)
{
    t_route     *route;
    t_route     *bf;

    route = routes->route;
    bf = before_fork->route->next;
    while (bf)
    {
        route->next = get_route(bf->room);
        route = route->next;
        bf = bf->next;
    }
    routes->rooms = before_fork->rooms;
}

t_routes    *get_routes(t_room *start)
{
    t_routes    *routes;

    if (!(routes = (t_routes *)malloc(sizeof(t_routes))))
        handle_error();
    routes->route = get_route(start);
    routes->rooms = 1;
    routes->next = NULL;
    return (routes);
}