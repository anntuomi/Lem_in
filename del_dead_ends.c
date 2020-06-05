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

static void		del_route(t_route **route, t_route *prev)
{
	t_route		*next;
	t_fork		*fork;
	t_fork		*tmp;

	next = (*route)->next;
	if (prev)
		prev->next = next;
	fork = (*route)->forks;
	while (fork)
	{
		tmp = fork;
		fork = fork->next;
		free(tmp);
	}
	free(*route);
	*route = next;
}

t_route			*del_dead_ends(t_route *route)
{
	t_route		*head;
	t_route		*prev;

	while (route && !route->rooms)
		del_route(&route, NULL);
	if (!route)
		handle_error();
	head = route;
	prev = route;
	route = route->next;
	while (route)
	{
		if (!route->rooms)
			del_route(&route, prev);
		else
		{
			route = route->next;
			prev = prev->next;
		}
	}
	return (head);
}
