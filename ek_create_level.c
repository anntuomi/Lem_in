/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ek_create_level.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuomine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/16 11:26:37 by atuomine          #+#    #+#             */
/*   Updated: 2020/06/16 11:26:53 by atuomine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static t_node	*make_node(t_room *room, t_room *prev_room, t_node *head,
t_node *prev)
{
	t_node		*node;

	if (!(node = (t_node *)malloc(sizeof(t_node) * 1)))
		return (NULL);
	node->room = room;
	node->prev_room = prev_room;
	node->next = head;
	node->previous = prev;
	return (node);
}

static t_node	*add_children(t_node *parent_node, t_node *head,
int *end_counter)
{
	t_path		*previous_connections;

	previous_connections = parent_node->room->paths;
	while (previous_connections && parent_node->room->type != END)
	{
		if (((t_room *)previous_connections->room)->used != 1 && \
		(previous_connections->flow == UNUSED || \
		previous_connections->flow == -1))
		{
			if (!(head = make_node(((t_room *)previous_connections->room), \
			parent_node->room, head, parent_node)))
				handle_error(0, "Malloc error");
			if (head->room->type == END)
				*end_counter = *end_counter + 1;
		}
		previous_connections = previous_connections->next;
	}
	return (head);
}

static void		mark_level_as_used(t_node *node_head)
{
	t_node		*node;

	node = node_head;
	while (node)
	{
		if (node->room->type != END)
			node->room->used = 1;
		node = node->next;
	}
}

t_level			*create_starting_level(t_room *start)
{
	t_path		*path;
	t_level		*level;

	if (!(level = (t_level *)malloc(sizeof(t_level) * 1)))
		handle_error(0, "Malloc error");
	level->next = NULL;
	level->nodes = NULL;
	level->end_counter = 0;
	path = start->paths;
	while (path)
	{
		if (path->flow == UNUSED)
		{
			if (!(level->nodes = make_node(((t_room *)path->room),
			start, level->nodes, NULL)))
				handle_error(0, "Malloc error");
			if (level->nodes->room->type == END)
				level->end_counter++;
		}
		path = path->next;
	}
	mark_level_as_used(level->nodes);
	return (level);
}

t_level			*create_level(t_level *previous)
{
	t_node		*parent_node;
	t_level		*new;
	t_path		*previous_connections;
	t_node		*head;

	if (!(new = (t_level *)malloc(sizeof(t_level) * 1)))
		handle_error(0, "Malloc error");
	new->next = NULL;
	parent_node = previous->nodes;
	head = NULL;
	new->end_counter = 0;
	while (parent_node)
	{
		head = add_children(parent_node, head, &new->end_counter);
		parent_node = parent_node->next;
	}
	new->nodes = head;
	mark_level_as_used(head);
	if (head == NULL)
	{
		free(new);
		new = NULL;
	}
	return (new);
}
