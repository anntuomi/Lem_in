#include "lemin.h"

static t_path   *get_path(char *room1, char *room2, t_room *room)
{
    t_path  *path;

    if (!(path = (t_path *)malloc(sizeof(t_path))))
            handle_error();
    while (room && !ft_strequ(room->name, room2))
        room = room->next;
    path->content = room;
    path->next = NULL;
    return (path);
}

static int      is_dup(char *name, t_path *path)
{
    t_room  *room;

    while (path)
    {
        room = path->content;
        if (ft_strequ(room->name, name))
            return (1);
        path = path->next;
    }
    return (0);
}

static void     set_path(t_link *link, t_room *room, t_room *rooms_head,
t_path **head, t_path **path)
{
    char    *name;
    int     match;

    if (ft_strequ(link->room1, room->name))
        match = 1;
    else if (ft_strequ(link->room2, room->name))
        match = 2;
    else
        match = 0;
    if (match > 0)
    {
        name = match == 1 ? link->room2 : link->room1;
        if (!*head)
        {
            *head = get_path(room->name, name, rooms_head);
            *path = *head;
        }
        else if (!is_dup(name, *head))
        {
            (*path)->next = get_path(room->name, name, rooms_head);
            *path = (*path)->next;
        }
    }
}

void            set_link(t_room *rooms_head, t_link *links_head)
{
    t_room  *room;
    t_link  *link;
    t_path  *head;
    t_path  *path;

    room = rooms_head;
    while (room)
    {
        head = NULL;
        path = NULL;
        link = links_head;
        while (link)
        {
            set_path(link, room, rooms_head, &head, &path);
            link = link->next;
        }
        room->paths = head;
        room = room->next;
    }
}