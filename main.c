#include "lemin.h"

static void		print_input(t_input *input)
{
	char	*output;
	char	*tmp;
	int		len;

	output = NULL;
	len = 0;
	while (input)
	{
		tmp = ft_strjoin_new(output, input->line, &len, '\n');
		if (output)
			free(output);
		output = tmp;
		input = input->next;
		if (len > 1000 || !input)
			print_output(&output, &len);
	}
	ft_putchar('\n');
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

static t_route	**get_ants(int *ant_count, t_input **input)
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
	*ant_count = (int)tmp;
	(*input)->line = line;
	(*input)->next = NULL;
	if (!(ants = (t_route **)malloc(sizeof(t_route *) * (*ant_count + 1))))
		handle_error();
	ants[*ant_count] = NULL;
	return (ants);
}

int				main(void)
{
	t_input		*head;
	t_input		*input;
	t_farm		farm;
	char		*line;

	if (!(head = (t_input *)malloc(sizeof(t_input))))
		handle_error();
	input = head;
	farm.ants = get_ants(&farm.ant_count, &input);
	create_room_list(&farm.rooms, &line, &input);
	if (!line)
		handle_error();
	find_edges(farm.rooms, &farm.start, &farm.end);
	set_links(line, farm.rooms, &input);
	farm.routes = get_routes_to_end(farm.start);
	print_input(head);
	farm.route_count = count_routes(farm.routes);
	farm.ordered = routes_to_array(farm.route_count, farm.routes);
	order_routes(farm.ordered);
	farm.start->ant_count = farm.ant_count;
	farm.needed_routes = count_needed_routes(farm.routes, farm.ordered[0]);
	solve(farm);
	return (0);
}
