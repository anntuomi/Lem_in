#include "lemin.h"

static t_path   *get_path(t_room *room)
{
    t_path  *path;

    if (!(path = (t_path *)malloc(sizeof(t_path))))
            handle_error();
    path->content = room;
    path->next = NULL;
    return (path);
}

static int      is_dup(int id, t_path *path)
{
    t_room  *room;

    while (path)
    {
        room = path->content;
        if (room->id == id)
            return (1);
        path = path->next;
    }
    return (0);
}

static void     set_path(t_link *link, int room1, t_path **head, t_path **path)
{
    t_room  *room2;

    if (link->room1->id == room1 && link->room2->id != room1)
        room2 = link->room2;
    else if (link->room2->id == room1 && link->room1->id != room1)
        room2 = link->room1;
    else
        room2 = NULL;
    if (room2)
    {
        if (!*head)
        {
            *head = get_path(room2);
            *path = *head;
        }
        else if (!is_dup(room2->id, *head))
        {
            (*path)->next = get_path(room2);
            *path = (*path)->next;
        }
    }
}

void            set_link(t_room *room, t_link *links_head)
{
    t_link  *link;
    t_path  *head;
    t_path  *path;

    while (room)
    {
        head = NULL;
        path = NULL;
        link = links_head;
        while (link)
        {
            set_path(link, room->id, &head, &path);
            link = link->next;
        }
        room->paths = head;
        room = room->next;
    }
}