/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuomine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/01 14:02:54 by atuomine          #+#    #+#             */
/*   Updated: 2020/06/01 14:02:56 by atuomine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void	ft_delete(char **array)
{
	int			i;

	i = 0;
	while (array[i] != NULL)
	{
		ft_strdel(&array[i]);
		i++;
	}
	free(array);
	array = NULL;
}

void	delete_group(t_group *group)
{
	t_route		**route;
	int			i;

	route = group->routes;
	i = 0;
	while (route[i])
	{
		free_route(route[i]);
		i++;
	}
	free(group->routes);
	free(group);
	group = NULL;
}

void	delete_branches(t_branch *branch)
{
	t_branch	*current;
	t_branch	*next;

	current = branch;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
}
