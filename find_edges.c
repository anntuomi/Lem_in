/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_edges.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuomine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/01 14:03:36 by atuomine          #+#    #+#             */
/*   Updated: 2020/06/01 14:03:37 by atuomine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static void		del_paths(t_path *path, t_path **head, int id)
{
	t_path		*prev;
	t_room		*tmp;

	free(path);
	prev = NULL;
	path = *head;
	while (path)
	{
		tmp = path->room;
		if (tmp->id == id)
		{
			if (!prev)
				*head = path->next;
			else
				prev->next = path->next;
			free(path);
			break ;
		}
		prev = path;
		path = path->next;
	}
}

static void		del_room(t_room **head, t_room **room, int flags)
{
	t_room		*cur;
	t_room		*next;

	cur = *room;
	while (cur && cur->type == NORMAL && (!cur->paths || !cur->paths->next))
	{
		if ((next = (cur->paths ? cur->paths->room : NULL)))
			del_paths(cur->paths, &next->paths, cur->id);
		if (!cur->prev)
			*head = cur->next;
		else
			cur->prev->next = cur->next;
		if (cur->next)
			cur->next->prev = cur->prev;
		if (*room && (*room)->id == cur->id)
			*room = (*room)->next;
		free(cur->name);
		free(cur);
		cur = next;
	}
	if (cur && cur->type != NORMAL && !cur->paths)
		handle_error(flags, "No paths");
}

void			find_edges(t_room **head, t_room **start, t_room **end,
int flags)
{
	t_room		*room;

	room = *head;
	*start = NULL;
	*end = NULL;
	while (room)
	{
		if (room->type == START && !*start)
			*start = room;
		else if (room->type == END && !*end)
			*end = room;
		else if (room->type != NORMAL)
			handle_error(flags, "Multiple ##start or ##end rooms");
		if (room->type != NORMAL && !room->paths)
			handle_error(flags, "No paths");
		else if (room->type == NORMAL && (!room->paths || !room->paths->next))
			del_room(head, &room, flags);
		else
			room = room->next;
	}
	if (!*start || !*end)
		handle_error(flags, "No ##start or/and ##end");
}
