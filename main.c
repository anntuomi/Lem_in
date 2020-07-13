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

void			handle_error(int flags, char *str)
{
	ft_putstr_fd("ERROR", 2);
	if (flags == ERROR || flags == ERROR_TURNS)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(str, 2);
	}
	ft_putstr_fd("\n", 2);
	exit(EXIT_FAILURE);
}

static t_ant	**get_ants(int *ant_count, t_input **input, int flags)
{
	t_ant		**ants;
	char		*line;
	long long	tmp;

	while (get_next_line(0, &line) == 1 && line[0] == '#')
	{
		if (line[1] != '#')
		{
			(*input)->line = line;
			if (!((*input)->next = (t_input *)malloc(sizeof(t_input))))
				handle_error(flags, "Malloc error");
			*input = (*input)->next;
		}
		else
			free(line);
	}
	if (!line || !ft_isnum(line) || (tmp = ft_atoll(line)) < 1 || tmp > INT_MAX)
		handle_error(flags, "Number_of_ants is missing or invalid");
	*ant_count = (int)tmp;
	(*input)->line = line;
	(*input)->next = NULL;
	if (!(ants = (t_ant **)malloc(sizeof(t_ant *) * (*ant_count + 1))))
		handle_error(flags, "Malloc error");
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
	if (!(head = (t_input *)malloc(sizeof(t_input))))
		handle_error(flags, "Malloc error");
	input = head;
	farm.ants = get_ants(&farm.ant_count, &input, flags);
	farm.rooms = NULL;
	create_room_list(&farm.rooms, &line, &input, flags);
	if (!line)
		handle_error(flags, "No links");
	set_links(line, farm.rooms, &input, flags);
	find_edges(&farm.rooms, &farm.start, &farm.end, flags);
	find_best_routes(&farm, flags);
	print_input(head, flags);
	order_routes(farm.ordered);
	farm.start->ant_count = farm.ant_count;
	solve(farm, flags);
	free_memory(farm);
	return (0);
}
