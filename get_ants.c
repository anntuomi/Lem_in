#include "lemin.h"

void    ants_to_start(t_route **ants, int amount, t_routes *shortest)
{
    int i;

    i = 0;
    while (i < amount)
    {
        if (!(ants[i] = (t_route *)malloc(sizeof(t_route))))
            handle_error();
        ants[i++] = shortest->route;
    }
    ants[i] = NULL;
}

t_route  **get_ants(int *amount)
{
    t_route  **ants;
    char    *line;

    while (get_next_line(0, &line) == 1 && line[0] == '#')
        free(line);
    if (!line || ft_isnum(line) != 1 || (*amount = ft_atoi(line)) < 1)
        handle_error();
    free(line);
    if (!(ants = (t_route **)malloc(sizeof(t_route *) * *amount)))
        handle_error();
    return (ants);
}