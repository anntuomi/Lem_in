#include "lemin.h"

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
        if (ant[i]->room->type != END && (ant[i]->next->room->ant_count != 0 && ant[i]->next->room->type != END))
        {
            route = route_list;
            while (route)
            {
                while(route->route->room->id != ant[i]->room->id)
                {
                    route->route = route->route->next;
                }
                if (route->route->next->room->ant_count == 0)
                {
                    ant[i] = route->route;
                    len = route->rooms - route->route->index;
                    route = route->next;
                    while (route)
                    {
                        if (route->rooms - route->route->index < len)
                        {
                            len = route->rooms - route->route->index;
                            ant[i] = route->route;
                        }
                        route = route->next;
                    }                    
                    break ;
                }        
                route = route->next;
            }
            if (route == NULL)
            {
                i++;
                continue ;
            }
        }
        if (ant[i]->room->type != END)
        {
            ant[i]->room->ant_count--;
            ant[i] = ant[i]->next;
            ant[i]->room->ant_count++;
            moved == 1 ? printf(" L%d-%s  %d", i + 1, ant[i]->room->name, ant[i]->index) : printf("L%d-%s %d", i + 1, ant[i]->room->name, ant[i]->index);
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