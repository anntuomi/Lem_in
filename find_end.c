#include "lemin.h"

int     count_paths(t_path *head, t_room **unvisited)
{
    int count;
    t_room *tmp;
    t_path *path;

    path = head;
    count = 0;
    while (path)
    {
        tmp = path->content;
        if (tmp->visited == 0)
        {
            count++;
            if (count == 1)
                *unvisited = tmp;
        }
        path = path->next;
    }
    return (count);
}

/*int     find_end(t_room *current, int len)
{
    int path_count;
    t_room *unvisited;
    t_room *fork;
    int shortest_len;
    int tmp_len;

    len++;
    current->visited = 1;
    if (current->type == END)
    {
        printf("End room: %s\n", current->name);
        current->visited = 0;
        return (len);
    }
    path_count = count_paths(current->paths, &unvisited);
    if (path_count > 1)
    {
        fork = current;
        shortest_len = len;
        tmp_len = len;
        if ((shortest_len = find_end(unvisited, len)) != 0)
        {
            while ((path_count = count_paths(fork->paths, &unvisited)) > 0)
            {
                if ((len = find_end(unvisited, len)) < shortest_len)
                    shortest_len = len;
                else
                    len = tmp_len;
            }
            return (shortest_len);
        }
        else
        {
            return (find_end(fork, len));
        }    
    }
    else if (path_count == 1)
    {
        return (find_end(unvisited, len));
    }
    return (0);
}*/

int     count_rooms(t_room *head)
{
    int len;
    t_room *current;
    t_room *next;
    t_path *path;

    current = head;
    len = 1;
    while (current->type != END)
    {
        path = current->paths;
        current->visited = 1;
            printf("%s!\n", current->name);
        while (path != NULL)
        {
            next = path->content;
            printf("Next visited: %s %d\n", next->name, next->visited);
            if (next->visited == 0)
                break ;
            path = path->next;
        }
        if (path == NULL)
        {
            len = 0;
            break ;
            current->next = NULL;
        }
        len++;
        current->next = path->content;
        current = path->content;
    }
    current = head;
    while (current && current->type != END)
    {
        current->visited = 0;
        current = current->next;
    }
    current->visited = 0;
    printf("branch len: %d\n", len);
    return (len);
}

t_room     *find_shortest(t_room *fork)
{
    int branch_len;
    t_room *next;
    t_path *path;
    int shortest_len;
    t_room *shortest;
    t_room *clear;

    shortest_len = 2147483647;
    path = fork->paths;
    while (path != NULL)
    {
        printf("Fork: %s\n", fork->name);
        next = path->content;
        if (next->visited == 0)
        {
            printf("Next: %s\n", next->name);
            branch_len = find_end(next, 0);
            printf("Branch len: %d\n", branch_len);
            if (branch_len < shortest_len && branch_len != 0)
            {
                shortest_len = branch_len;
                shortest = next;
            }
        }
        path = path->next;
    }
    return (shortest);
}

int     find_end(t_room *head, int len)
{
    int path_count;
    t_room *unvisited;
    t_room *fork;
    t_room *current;
    int branch_len;
    t_room *next;
    t_room *clear;

    current = head;
    while (1)
    {
        len++;
        if (current->type == END)
        {
            current->next = NULL;
            printf("Current: %s len: %d\n", current->name, len);
            return (len);
        }
        current->visited = 1;
        path_count = count_paths(current->paths, &unvisited);
        if (path_count > 1)
        {
            printf("Current: %s\n", current->name);
            next = find_shortest(current);
            clear = next;
            while (clear)
            {
                printf("Clearing: %s\n", clear->name);
                clear->visited = 0;
                clear = clear->next;
            }
            current->next = next;
            current = next;
        }
        else if (path_count == 1)
        {
            printf("Current: %s\n", current->name);
            current->next = unvisited;
            current = unvisited;
        }
        else
        {
            break ;
        }       
    }
    return (0);
}