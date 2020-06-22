/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuomine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/01 14:03:36 by atuomine          #+#    #+#             */
/*   Updated: 2020/06/01 14:03:37 by atuomine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void			handle_error(void)
{
	ft_putstr_fd("ERROR\n", 2);
	exit(EXIT_FAILURE);
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
	return (ants);
}

static void		print_branches(t_branch *branch)
{
	t_route		*route;
	t_fork		*fork;
	int			branches;
	int			routes;
	int			forks;

	branches = 1;
	while (branch)
	{
		printf("%d. Branch (%d route(s))\n", branches, branch->routes);
		route = branch->route;
		routes = 1;
		while (route)
		{
			printf("%d.%d Route (%d room(s))\n", branches, routes,
			route->rooms);
			fork = route->forks;
			forks = 1;
			while (fork)
			{
				printf("%d.%d.%d Fork (from %s to %s)\n", branches, routes,
				forks++, fork->from->name, fork->to->name);
				fork = fork->next;
			}
			routes++;
			if (route->next)
				printf("\n");
			route = route->next;
		}
		branches++;
		if (branch->next)
			printf("\n\n");
		branch = branch->next;
	}
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
	farm.rooms = NULL;
	create_room_list(&farm.rooms, &line, &input);
	if (!line)
		handle_error();
	find_edges(farm.rooms, &farm.start, &farm.end);
	set_links(line, farm.rooms, &input);
	farm.branches = get_branches_to_end(farm.start);
	print_branches(farm.branches);
	/*print_input(head);
	farm.route_count = count_routes(farm.routes);
	farm.ordered = routes_to_array(farm.route_count, farm.routes);
	order_routes(farm.ordered);
	farm.start->ant_count = farm.ant_count;
	farm.path_count = count_max_path_count(farm.routes, farm.ordered[0]);
	solve(farm);
	free_memory(farm);*/
	return (0);
}
