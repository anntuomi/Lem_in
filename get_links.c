#include "lemin.h"

static int      count_words(char **array)
{
    int     words;

    words = 0;
    while (array[words])
        words++;
    return (words);
}

static t_link   *get_link(char *line)
{
    t_link  *link;
    char    **rooms;

    if (!(link = (t_link *)malloc(sizeof(t_link))) ||
    !(rooms = ft_strsplit(line, '-')) || count_words(rooms) != 2)
        handle_error();
    free(line);
    link->room1 = rooms[0];
    link->room2 = rooms[1];
    free(rooms);
    link->next = NULL;
    return (link);
}

t_link          *get_links(char *line)
{
    t_link  *head;
    t_link  *link;
    char    **rooms;

    head = get_link(line);
    link = head->next;
    while (get_next_line(0, &line) == 1)
    {
        link = get_link(line);
        link = link->next;
    }
    link = head;
    while (link)
    {
        printf("%s-%s\n", link->room1, link->room2);
        link = link->next;
    }
    return (head);
}