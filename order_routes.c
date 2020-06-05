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

void			order_routes(t_route **routes)
{
	t_route		*tmp;
	int			i;
	int			j;

	i = 0;
	while (routes[i])
	{
		j = i + 1;
		while (routes[j])
		{
			if (routes[j]->rooms < routes[i]->rooms)
			{
				tmp = routes[i];
				routes[i] = routes[j];
				routes[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

t_route			**routes_to_array(int route_count, t_route *routes)
{
	t_route		**array_routes;
	int			i;

	i = 0;
	if (!(array_routes = (t_route **)malloc(sizeof(t_route *) *
	(route_count + 1))))
		handle_error();
	while (routes)
	{
		array_routes[i] = routes;
		i++;
		routes = routes->next;
	}
	array_routes[i] = NULL;
	return (array_routes);
}

int				count_routes(t_route *routes)
{
	int			count;

	count = 0;
	while (routes)
	{
		routes = routes->next;
		count++;
	}
	return (count);
}
