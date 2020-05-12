#include "lemin.h"

/*static void		print_routes(t_routes **routes)
{
	t_route		*route;
	int			i;

	i = 0;
	while (routes[i])
	{
		printf("%d. route (%d rooms)\n", i + 1, routes[i]->rooms);
		route = routes[i]->route;
		while (route)
		{
			printf("%d. %s\n", route->index, route->room->name);
			route = route->next;
		}
		i++;
	}
	printf("\n");
}*/

static void		print_input(t_input *input)
{
	char	*output;
	char	*tmp;
	int		len;

	len = 0;
	output = NULL;
	while (input)
	{
		tmp = ft_append(output, input->line, &len, '\n');
		if (output != NULL)
			free(output);
		output = tmp;
		input = input->next;
		if (len > 1000 || input == NULL)
			print_output(&output, &len);
	}
	write(1, "\n", 1);
}

static void		find_edges(t_room *room, t_room **start, t_room **end)
{
	*start = NULL;
	*end = NULL;
	while (room)
	{
		if (room->type == START && !*start)
			*start = room;
		else if (room->type == END && !*end)
			*end = room;
		else if (room->type == START || room->type == END)
			handle_error();
		room = room->next;
	}
	if (!*start || !*end)
		handle_error();
}

void			set_input(t_input **input, char *line, int rooms)
{
	if (line[0] == '#' && line[1] == '#' &&
	((rooms && determine_room_type(line) == NORMAL) || !rooms))
		free(line);
	else
	{
		if (!((*input)->next = (t_input *)malloc(sizeof(t_input))))
			handle_error();
		*input = (*input)->next;
		(*input)->line = line;
		(*input)->next = NULL;
	}
}

/*void			count_room_routes(t_routes *routes_head, t_room *room_list)
{
	t_room		*room;
	t_route		*current;
	t_routes	*route;

	room = room_list;
	while (room)
	{
		if (room->type != START && room->type != END)
		{
			route = routes_head;
			while (route)
			{
				current = route->route;
				while (current)
				{
					if (current->room->id == room->id)
						room->route_count++;
					current = current->next;
				}
				current = route->route;
				route = route->next;
			}
		}
		room = room->next;
	}
}*/

static t_route	**get_ants(int *amount, t_input **input)
{
	t_route		**ants;
	char		*line;
	long long	tmp;

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
	if (!line || !ft_isnum(line) || (tmp = ft_atoll(line)) < 1 || tmp > INT_MAX)
		handle_error();
	*amount = (int)tmp;
	(*input)->line = line;
	(*input)->next = NULL;
	if (!(ants = (t_route **)malloc(sizeof(t_route *) * (*amount + 1))))
		handle_error();
	ants[*amount] = NULL;
	return (ants);
}

int				main(void)
{
	t_input		*head;
	t_input		*input;
	t_farm		farm;
	char		*line;
	t_routes	**routes_to_use;

	if (!(head = (t_input *)malloc(sizeof(t_input))))
		handle_error();
	input = head;
	farm.ants = get_ants(&farm.ant_count, &input);
	create_room_list(&farm.rooms, &line, &input);
	if (!(line))
		handle_error();
	find_edges(farm.rooms, &farm.start, &farm.end);
	set_links(line, farm.rooms, &input);
	farm.routes = get_routes_to_end(farm.start);
	print_input(head);
	farm.count = count_routes(farm.routes);
	farm.ordered = routes_to_array(farm.count, farm.routes);
	order_routes(farm.ordered);
	farm.start->ant_count = farm.ant_count;
	farm.max_path_count = count_max_path_count(farm.routes, farm.ordered[0]);
	solve(farm, farm.ordered, farm.max_path_count);
	return (0);
}
