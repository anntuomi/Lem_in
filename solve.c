/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuomine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/01 14:04:15 by atuomine          #+#    #+#             */
/*   Updated: 2020/06/01 14:04:17 by atuomine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

int				calculate_moves(t_route **routes, int path_count,
int ant_count)
{
	int			moves;
	int			rooms;
	int			i;

	rooms = 0;
	i = 0;
	while (i < path_count)
		rooms += routes[i++]->rooms - 2;
	moves = (ant_count + rooms) / path_count +
	((ant_count + rooms) % path_count ? 1 : 0);
	return (moves);
}

static void		assign_paths(t_farm farm, int flags)
{
	int			moves;
	int			i;
	int			j;

	moves = calculate_moves(farm.ordered, farm.route_count, farm.ant_count);
	i = 0;
	j = 0;
	while (i < farm.ant_count)
	{
		if (j == farm.route_count || farm.ordered[j]->rooms - 1 > moves)
		{
			j = 0;
			moves--;
		}
		if (!(farm.ants[i] = (t_ant *)malloc(sizeof(t_ant))))
			handle_error(flags, "Malloc error");
		farm.ants[i]->forks = farm.ordered[j]->forks;
		farm.ants[i]->room = farm.start;
		farm.ants[i]->prev = NULL;
		farm.ants[i++]->fork = farm.ordered[j++]->forks;
	}
	farm.ants[i] = NULL;
	free(farm.ordered);
}

char			*solve(t_farm farm, int flags)
{
	char		*output;
	char		*line;
	char		*tmp;
	int			len;
	int			turns;

	assign_paths(farm, flags);
	output = NULL;
	len = 0;
	turns = 0;
	while (farm.end->ant_count != farm.ant_count)
	{
		line = move_ants(farm.ants, flags);
		tmp = ft_append(output, line, &len, '\n', flags);
		if (output)
			free(output);
		output = tmp;
		free(line);
		if (len > 1000 || farm.end->ant_count == farm.ant_count)
			print_output(&output, &len);
		turns++;
	}
	if (flags == TURNS || flags == ERROR_TURNS)
	{
		ft_putstr("\nTurns: ");
		ft_putnbr(turns);
		ft_putchar('\n');
	}
	char *result = ft_itoa(turns);
	return (result);
}
