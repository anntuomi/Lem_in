#include "lemin.h"

static int      count_words(char **array)
{
    int     words;

    words = 0;
    while (array[words])
        words++;
    return (words);
}

void	save_link(t_link **links, char *line)
{
    t_link *new;
    char **input;

    if (!(new = (t_link *)malloc(sizeof(t_link))) ||
    !(input = ft_strsplit(line, '-')) ||
     count_words(input) != 2)
        handle_error();
    new->room1 = ft_strdup(input[0]);
    new->room2 = ft_strdup(input[1]);
    ft_2ddel(input);
	new->next = *links;
	*links = new;
}

void    handle_links(t_link **links, char *line)
{
    *links = NULL;
    save_link(links, line);
    free(line);
    while (get_next_line(0, &line))
    {
        if (line)
            save_link(links, line);
        free(line);
    }
}