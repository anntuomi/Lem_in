#include "lemin.h"

void    ants_to_start(t_room **ants, int amount, t_room *start)
{
    int i;

    i = 0;
    while (i < amount)
    {
        if (!(ants[i] = (t_room *)malloc(sizeof(t_room))))
            handle_error();
        ants[i++] = start;
    }
    ants[i] = NULL;
}

t_room  **get_ants(int *amount)
{
    t_room  **ants;
    char    *line;

    if (get_next_line(0, &line) != 1 || ft_isnum(line) != 1 ||
    (*amount = ft_atoi(line)) < 1)
        handle_error();
    if (!(ants = (t_room **)malloc(sizeof(t_room *) * *amount)))
        handle_error();
    return (ants);
}