#include "lemin.h"

static void	print_routes(t_routes **routes)
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
			printf("%d. %s %d\n", route->index, route->room->name,
			route->room->route_count);
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

void		count_room_routes(t_routes *routes_head, t_room *room_list)
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
	count_room_routes(farm.routes, farm.rooms);
	farm.ordered = routes_to_array(farm.count, farm.routes);
	order_routes(farm.ordered);
	//print_routes(farm.ordered);
	farm.start->ant_count = farm.amount;
	//printf("path count %d\n",
	farm.needed_routes = count_needed_routes(farm.routes, farm.ordered[0]);
	solve(farm, farm.ordered, farm.needed_routes);
	ants_to_end(farm);
	return (0);
}
