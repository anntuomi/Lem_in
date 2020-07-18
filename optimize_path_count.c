/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_optimized_path_count.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuomine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/01 14:02:54 by atuomine          #+#    #+#             */
/*   Updated: 2020/06/01 14:02:56 by atuomine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static void		alter_group(t_group *group, int least_pc, int least_moves)
{
	int		i;
	t_route	**shortened;

	i = 0;
	if (least_pc < least_moves)
	{
		if (!(shortened = (t_route **)malloc(sizeof(t_route *) *
		(least_pc + 1))))
			handle_error(0, "Malloc error");
		while (i < least_pc)
		{
			shortened[i] = group->routes[i];
			i++;
		}
		shortened[i] = NULL;
		while (group->routes[i])
		{
			free_route(group->routes[i]);
			i++;
		}
		free(group->routes);
		group->routes = shortened;
		group->path_count = least_pc;
		group->moves = least_moves;
	}
}

void			optimize_path_count(t_group *group, int ant_count,
int all_moves)
{
	t_route	*tmp;
	int		moves;
	int		least_moves;
	int		least_pc;
	int		new_path_count;

	least_moves = all_moves;
	least_pc = group->path_count;
	new_path_count = 1;
	order_routes(group->routes);
	while (new_path_count <= group->path_count)
	{
		tmp = group->routes[new_path_count];
		group->routes[new_path_count] = NULL;
		moves = calculate_moves(group->routes, new_path_count, ant_count);
		if (moves < least_moves)
		{
			least_moves = moves;
			least_pc = new_path_count;
		}
		group->routes[new_path_count] = tmp;
		new_path_count++;
	}
	alter_group(group, least_pc, least_moves);
}
