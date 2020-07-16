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

void			order_routes(t_route **array)
{
	t_route		*tmp;
	int			i;
	int			j;

	i = 0;
	while (array[i])
	{
		j = i + 1;
		while (array[j])
		{
			if (array[j]->rooms < array[i]->rooms)
			{
				tmp = array[i];
				array[i] = array[j];
				array[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

t_route			**routes_to_array(int count, t_branch *branch)
{
	t_route		**array;
	int			i;

	if (!(array = (t_route **)malloc(sizeof(t_route *) * (count + 1))))
		handle_error(0, "Malloc error");
	i = 0;
	while (branch)
	{
		array[i++] = branch->route;
		branch = branch->next;
	}
	array[i] = NULL;
	return (array);
}

int				count_routes(t_branch *branch)
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
