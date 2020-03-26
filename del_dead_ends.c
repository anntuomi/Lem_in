#include "lemin.h"

static void del_route(t_route *route)
{
    t_route *tmp;
    while (route)
    {
        tmp = route;
        route = route->next;
        free(tmp);
    }
}

t_routes    *del_dead_ends(t_routes *routes)
{
    t_routes    *head;
    t_routes    *previous;

    while (routes->rooms == 0)
    {
        previous = routes;
        routes = routes->next;
        del_route(previous->route);
        free(previous);
    }
    head = routes;
    previous = routes;
    routes = routes->next;
    while (routes)
    {
        if (routes->rooms == 0)
        {
            previous->next = routes->next;
            del_route(routes->route);
            free(routes);
        }
        previous = routes;
        routes = previous->next;
    }
    return (head);
}