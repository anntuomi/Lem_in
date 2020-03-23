#ifndef LEMIN_H
# define LEMIN_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <fcntl.h>
# include "libft.h"
# include "get_next_line.h"

# define START 0
# define NORMAL 1
# define END 2
# define NOTROOM 0
# define ISROOM 1

typedef struct  s_path
{
	void    *content;
	void    *next;
}               t_path;

typedef struct  s_room
{
    char    *name;
    int     x;
    int     y;
    int     type;
    int     id;
    t_path  *paths;
    void    *next;
}               t_room;

typedef struct  s_link
{
    char    *room1;
    char    *room2;
    void    *next;
}               t_link;

typedef struct  s_farm
{
    t_room  **ants;
    t_room  *rooms;
    t_link  *links;
    t_room  *start;
    t_room  *end;
}               t_farm;

t_room          **get_ants(int *amount);
void            create_room_list(t_room **head, char **line);
void            ants_to_start(t_room **ants, int amount, t_room *start);
t_link          *get_links(char *line, t_room *room);
void            set_link(t_room *rooms_head, t_link *links_head);
int             find_end(t_room *current, char *visited);
void            handle_error(void);

#endif