#include "lemin.h"

int     count_paths(t_path *head, char *visited, t_room **unvisited)
{
    int count;
    t_room *tmp;
    t_path *path;

    path = head;
    count = 0;
    while (path)
    {
        tmp = path->content;
        if (!strstr(visited, tmp->name))
        {
            count++;
            if (count == 1)
                *unvisited = tmp;
        }
        path = path->next;
    }
    return (count);
}

int     find_end(t_room *current, char *visited)
{
    int path_count;
    t_room *unvisited;
    t_room *fork;

    //end recursion if we are at the end room
    if (current->type == END)
        return (1);
    //Set up the list of visited rooms in the beginning
    if (visited == NULL)
    {
        visited = (char *)malloc(sizeof(char) * 1);
        visited[0] = '\0';
    }
    //Join current room to the list of visited rooms
    visited = ft_strjoin(visited, current->name);
    //Count how many paths current room has, and save the first unvisited room you
    //can access from current room to *unvisited
    path_count = count_paths(current->paths, visited, &unvisited);
    if (path_count > 1)
    {
        //If you can access more than 1 room from current room,
        //save current room as a "crossroads" or a fork in the route.
        fork = current;
        //Goes to the unvisited room we saved earlier and checks
        //if you can access the end room from current room
        if (find_end(unvisited, visited) == 1)
            return (1);
        else
        {
            //If the tested room doesn't lead to the end room,
            //Save it to visited rooms, and return to the fork room, and
            //try again with the next path.
            visited = ft_strjoin(visited, unvisited->name);
            return (find_end(fork, visited));
        }    
    }
    else if (path_count == 1)
    {
        //Simply move to the next room if there is only unvisited
        //path in the current room
        visited = ft_strjoin(visited, unvisited->name);
        return (find_end(unvisited, visited));
    }
    //Function will return 0 if there are no unvisited paths left.
    return (0);
}