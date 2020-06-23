/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   order_routes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuomine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/01 14:04:02 by atuomine          #+#    #+#             */
/*   Updated: 2020/06/01 14:04:05 by atuomine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void			order_routes(t_branch **branch, t_route *tmp)
{
	int			i;
	int			ii;
	int			iii;

	i = 0;
	while (branch[i])
	{
		ii = 0;
		while (branch[i]->array[ii])
		{
			iii = ii + 1;
			while (branch[i]->array[iii])
			{
				if (branch[i]->array[iii]->rooms < branch[i]->array[ii]->rooms)
				{
					tmp = branch[i]->array[ii];
					branch[i]->array[ii] = branch[i]->array[iii];
					branch[i]->array[iii] = tmp;
				}
				iii++;
			}
			ii++;
		}
		i++;
	}
}

t_branch		**branches_to_array(int count, t_branch *branch, t_route *route)
{
	t_branch	**array;
	int			i;
	int			j;

	if (!(array = (t_branch **)malloc(sizeof(t_branch *) * (count + 1))))
		handle_error();
	i = 0;
	while (branch)
	{
		array[i] = branch;
		if (!(array[i]->array = (t_route **)malloc(sizeof(t_route *) *
		(array[i]->routes + 1))))
			handle_error();
		j = 0;
		route = array[i]->route;
		while (route)
		{
			array[i]->array[j++] = route;
			route = route->next;
		}
		array[i++]->array[j] = NULL;
		branch = branch->next;
	}
	array[i] = NULL;
	return (array);
}

int				count_branches(t_branch *branch)
{
	int			count;

	count = 0;
	while (branch)
	{
		branch = branch->next;
		count++;
	}
	return (count);
}
