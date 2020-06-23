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

static void		print_branches(t_branch **branch)
{
	t_route		**route;
	t_fork		*fork;
	int			i;
	int			ii;
	int			iii;

	i = 0;
	while (branch[i])
	{
		printf("%d. Branch (%d route(s))\n", i + 1, branch[i]->routes);
		route = branch[i]->array;
		ii = 0;
		while (route[ii])
		{
			printf("%d.%d Route (%d room(s))\n", i + 1, ii + 1,
			route[ii]->rooms);
			fork = route[ii]->forks;
			iii = 1;
			while (fork)
			{
				printf("%d.%d.%d Fork (from %s to %s)\n", i + 1, ii + 1, iii++,
				fork->from->name, fork->to->name);
				fork = fork->next;
			}
			if (route[++ii])
				printf("\n");
		}
		if (branch[++i])
			printf("\n\n");
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
	//print_input(head);
	farm.branch_count = count_branches(farm.branches);
	farm.ordered = branches_to_array(farm.branch_count, farm.branches, NULL);
	order_routes(farm.ordered, NULL);
	print_branches(farm.ordered);
	/*farm.start->ant_count = farm.ant_count;
	farm.path_count = count_max_path_count(farm.routes, farm.ordered[0]);
	solve(farm);
	free_memory(farm);*/
	return (0);
}
