#include "lemin.h"

void    add_start_room(char **ants, int amount, t_room *room)
{
    int     i;

    while (room && room->type != 0)
        room = room->next;
    i = 0;
    while (i < amount)
    {
        if (!(ants[i++] = ft_strdup(room->name)))
            handle_error();
    }
    ants[i] = NULL;
}

char    **get_ants(int *amount)
{
    char    **ants;
    char    *line;

    if (get_next_line(0, &line) != 1 || ft_isnum(line) != 1 ||
    (*amount = ft_atoi(line)) < 1)
        handle_error();
    if (!(ants = (char **)malloc(sizeof(char *) * *amount)))
        handle_error();
    return (ants);
}