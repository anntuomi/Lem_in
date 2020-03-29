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
    int ant_nbr;

    i = amount - 1;
    ant_nbr = 1;
    moved = 0;
    while (i >= 0)
    {
  /*  	if (ant[i]->room->type != END && (ant[i]->next->room->ant_count != 0 \
        	&& ant[i]->next->room->type != END))
            ant[i] = change_route(ant[i], route_list);*/
        if (ant[i]->room->type != END && (ant[i]->next->room->ant_count == 0 \
        	|| ant[i]->next->room->type == END))
        {
            ant[i]->room->ant_count--;
            ant[i] = ant[i]->next;
            ant[i]->room->ant_count++;
            moved == 1 ? printf(" L%d-%s", ant_nbr, ant[i]->room->name) : \
            printf("L%d-%s", ant_nbr, ant[i]->room->name);
            moved = 1;
        }
        i--;
        ant_nbr++;
    }
    printf("\n");
}

int        count_paths(t_room *start, t_room *end)
{
    t_path *path;
    int     path_count;

    path = end->paths;
    path_count = 0;
    while (path)
    {
        path_count++;
        path = path->next;
    }
    return (path_count);
}

t_route        **assign_paths(t_route **ant, t_routes **ordered, int path_count, int amount)
{
    int i;

    i = 0;
    while (i < amount)
    {
        printf("Ant number %d (Path_count: %d) Mod: %d\n", i, path_count, (i) % path_count);
        ant[i] = ordered[(i) % path_count]->route;
        i++;
    }
    return (ant);
}

void        solve(t_farm farm, t_routes **ordered, int total_paths)
{
	int	move_count;
    int path_count;

    path_count = count_paths(farm.start, farm.end);
    path_count = (path_count > total_paths ? total_paths : path_count);
    move_count = 0;
    farm.ants = assign_paths(farm.ants, ordered, path_count, farm.amount);
    while (farm.end->ant_count != farm.amount)
    {
        move_ants(farm.amount, farm.ants, farm.routes);
		move_count++;
    }
	printf("Move count: %d\n", move_count);
}