/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del_dead_ends.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuomine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/01 14:02:04 by atuomine          #+#    #+#             */
/*   Updated: 2020/06/01 14:02:08 by atuomine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static t_routes	*del_route(t_routes *routes)
{
	t_routes	*next;
	t_route		*route;
	t_route		*tmp;

	next = routes->next;
	route = routes->route;
	while (route)
	{
		tmp = route;
		route = route->next;
		free(tmp);
	}
	free(routes);
	return (next);
}

t_routes		*del_dead_ends(t_routes *route)
{
	t_routes	*head;
	t_routes	*previous;

	while (route && !route->rooms)
		route = del_route(route);
	if (!route)
		handle_error();
	head = route;
	previous = route;
	route = route->next;
	while (route)
	{
		if (!route->rooms)
			previous->next = del_route(route);
		else
			previous = previous->next;
		route = previous->next;
	}
	return (head);
}