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

typedef struct	s_path
{
	void	*content;
	void    *next;
}				t_path;

typedef struct	s_room
{
    char    *name;
    int     x;
    int     y;
    int     type;
    t_path  *paths;
    void    *next;
}				t_room;

typedef struct	s_link
{
    char    *room1;
    char    *room2;
    void    *next;
}				t_link;

typedef struct	s_farm
{
    char    **ants;
    t_room  *rooms;
    t_link  *links;
    t_room  *start;
    t_room  *end;
}				t_farm;

char            **get_ants(int *amount);
void            create_room_list(t_room **rooms, char **line);
void            ants_to_start(char **ants, int amount, t_room *room);
void            handle_links(t_link **links, char *line);
void            handle_error(void);
t_link          *get_links(char *line, t_room *room);

#endif