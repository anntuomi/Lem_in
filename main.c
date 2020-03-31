#include "lemin.h"

static void	print_routes(t_routes **routes, int route_count)
{
	t_route		*route;
	int			i;

	i = 0;
	while (i < route_count)
	{
		printf("route (%d rooms)\n", routes[i]->rooms);
		route = routes[i]->route;
		while (route)
		{
			printf("%d. %s\n", route->index, route->room->name);
			route = route->next;
		}
		i++;
	}
	printf("\n");
}

static void	print_input(t_input *input)
{
	while (input)
	{
		printf("%s\n", input->line);
		input = input->next;
	}
	printf("\n");
}

static void	find_edges(t_room *room, t_room **start, t_room **end)
{
	*start = NULL;
	*end = NULL;
	while (room)
	{
		if (room->type == START)
			*start = room;
		if (room->type == END)
			*end = room;
		room = room->next;
	}
	if (*start == NULL || *end == NULL)
		handle_error();
}

void		set_input(t_input **input, char *line, int rooms)
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

int			main(void)
{
	t_input		*head;
	t_input		*input;
	t_farm		farm;
	char		*line;

	if (!(head = (t_input *)malloc(sizeof(t_input))))
		handle_error();
	input = head;
	farm.ants = get_ants(&farm.amount, &input);
	create_room_list(&farm.rooms, &line, &input);
	if (!(line))
		handle_error();
	find_edges(farm.rooms, &farm.start, &farm.end);
	set_links(line, farm.rooms, &input);
	farm.routes = get_routes_to_end(farm.start);
	print_input(head);
	farm.count = count_routes(farm.routes);
	farm.ordered_routes = order_routes(farm.count, farm.routes);
	print_routes(farm.ordered_routes, farm.count);
	farm.start->ant_count = farm.amount;
	solve(farm, farm.ordered_routes, farm.count);
	return (0);
}
