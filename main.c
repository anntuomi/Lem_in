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

static int		get_nbr_of_ants(char *line)
{
	long long tmp;

	tmp = 0;
	if (!line || !ft_isnum(line) || (tmp = ft_atoll(line)) < 1 ||
	tmp > INT_MAX || ft_strlen(line) > 10)
		handle_error(0, "Number_of_ants is missing or invalid");
	return ((int)tmp);
}

static t_ant	**get_ants(int *ant_count, t_input **input)
{
	t_ant		**ants;
	char		*line;

	(*input)->line = NULL;
	(*input)->next = NULL;
	while (get_next_line(0, &line) == 1 && line[0] == '#')
		set_input(input, line);
	*ant_count = get_nbr_of_ants(line);
	set_input(input, line);
	if (!(ants = (t_ant **)malloc(sizeof(t_ant *) * (*ant_count + 1))))
		handle_error(0, "Malloc error");
	return (ants);
}

static int		get_flags(int ac, char **av)
{
	int			error;
	int			turns;
	int			i;

	error = 0;
	turns = 0;
	while (--ac >= 0)
	{
		if (av[ac][0] == '-')
		{
			i = 1;
			while (av[ac][i])
			{
				if (av[ac][i] == 'e' && !error)
					error = 1;
				else if (av[ac][i] == 't' && !turns)
					turns = 2;
				i++;
			}
		}
	}
	return (error + turns);
}

int				main(int ac, char **av)
{
	int			flags;
	t_input		*head;
	t_input		*input;
	t_farm		farm;
	char		*line;

	flags = get_flags(ac, av);
	handle_error(flags, SET_FLAGS_FOR_FUTURE_USE);
	if (!(head = (t_input *)malloc(sizeof(t_input))))
		handle_error(0, "Malloc error");
	input = head;
	farm.ants = get_ants(&farm.ant_count, &input);
	farm.rooms = NULL;
	create_room_list(&farm.rooms, &line, &input);
	if (!line)
		handle_error(0, "No links");
	set_links(line, farm.rooms, &input);
	find_edges(&farm.rooms, &farm.start, &farm.end);
	find_best_routes(&farm);
	print_input(head);
	order_routes(farm.ordered);
	farm.start->ant_count = farm.ant_count;
	solve(farm, flags);
	free_memory(farm);
	return (0);
}
