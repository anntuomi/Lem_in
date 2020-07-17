/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_dfs_forks.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuomine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/01 14:02:54 by atuomine          #+#    #+#             */
/*   Updated: 2020/06/01 14:02:56 by atuomine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

int				check_if_end_path(t_room *room)
{
	t_path *current;

	current = room->paths;
	while (current)
	{
		if (((t_room *)current->room)->type == END && current->flow == 1)
			return (1);
		current = current->next;
	}
	return (0);
}

void			clear_visited(t_fork *head)
{
	t_fork *current;

	current = head;
	while (current)
	{
		current->from->visited = 0;
		current->to->visited = 0;
		current = current->next;
	}
}

t_room			*find_next_room(t_room *current, t_room *end, t_fork *fork,
t_fork *fork_head)
{
	t_path *path;

	if (current->connection == END && check_if_end_path(current))
		return (end);
	else
	{
		path = current->paths;
		while (path && (path->flow != 1 ||
		(path->flow == 1 && ((t_room *)path->room)->visited == 2) ||
		(path->flow == 1 && ((t_room *)path->room)->connection == START)))
			path = path->next;
		if (path == NULL)
		{
			fork->next = NULL;
			clear_visited(fork_head);
			delete_forks(fork_head);
			return (NULL);
		}
		return ((t_room *)path->room);
	}
}

t_fork			*route_error(int *length)
{
	*length = 0;
	return (NULL);
}

t_fork			*save_dfs_forks(t_room *start, t_room *current, int *length,
t_room *end)
{
	t_fork	*fork_head;
	t_fork	*fork;
	t_path	*path;
	t_room	*previous;

	if (current->visited == 2)
		return (route_error(length));
	fork = create_fork(start, current);
	if (current->type != END)
		current->visited = 2;
	fork_head = fork;
	path = current->paths;
	while (current->type != END)
	{
		previous = current;
		if (!(current = find_next_room(current, end, fork, fork_head)))
			return (route_error(length));
		if (current->type != END)
			current->visited = 2;
		fork->next = create_fork(previous, current);
		*length = *length + 1;
		fork = fork->next;
	}
	fork->next = NULL;
	return (fork_head);
}
