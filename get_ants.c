#include "lemin.h"

t_route	**get_ants(int *amount, t_input **input)
{
	t_route	**ants;
	char	*line;

	while (get_next_line(0, &line) == 1 && line[0] == '#')
	{
		if (line[1] != '#')
		{
			(*input)->line = line;
			if (!((*input)->next = (t_input *)malloc(sizeof(t_input))))
				handle_error();
			*input = (*input)->next;
		}
		else
			free(line);
	}
	if (!line || ft_isnum(line) != 1 || (*amount = ft_atoi(line)) < 1)
		handle_error();
	(*input)->line = line;
	(*input)->next = NULL;
	if (!(ants = (t_route **)malloc(sizeof(t_route *) * *amount)))
		handle_error();
	return (ants);
}
