#ifndef LEMIN_H
# define LEMIN_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "libft.h"
#include "get_next_line.h"

#define START 0
#define NORMAL 1
#define END 2
#define NOTROOM 0
#define ISROOM 1

typedef struct	s_room
{
    char *name;
    int x;
    int y;
    int type;
    void *next;
}				t_room;

typedef struct	s_link
{
    char    *room1;
    char    *room2;
    void    *next;
}				t_link;

void            create_room_list(t_room **rooms, char **line);
void            get_links(t_link **links, char *line);
void            handle_error(void);

#endif