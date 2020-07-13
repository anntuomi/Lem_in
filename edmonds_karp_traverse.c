/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_branches.c                                    :+:      :+:    :+:   */
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

static t_level	*create_starting_level(t_room *start, int flags)
{
	t_node		*head;
	t_path		*connection;
	t_level		*level;

	if (!(level = (t_level *)malloc(sizeof(t_level) * 1)))
		handle_error(flags, "Malloc error");
	level->next = NULL;
	level->size = 0;
	head = NULL;
	level->end_counter = 0;
	connection = start->paths;
	while (connection)
	{
		if (connection->flow == UNUSED)
		{
			if (!(head = make_node(((t_room *)connection->room), start, head,
			NULL)))
				handle_error(flags, "Malloc error");
			if (head->room->type == END)
				level->end_counter++;
			level->size++;
		}
		connection = connection->next;
	}
	level->nodes = head;
	level->depth = 1;
	if (level->size == 0)
	{
		free(level);
		return (NULL);
	}
	return (level);
}

static t_node	*add_children(t_node *parent_node, t_node *head,
int *end_counter, int flags)
{
	t_path		*previous_connections;

	previous_connections = parent_node->room->paths;
	while (previous_connections && parent_node->room->type != END)
	{
		if (((t_room *)previous_connections->room)->used != 2 && \
		(previous_connections->flow == UNUSED || \
		previous_connections->flow == -1))
		{
			if (!(head = make_node(((t_room *)previous_connections->room), \
			parent_node->room, head, parent_node)))
				handle_error(flags, "Malloc error");
			if (head->room->type == END)
				*end_counter = *end_counter + 1;
		}
		previous_connections = previous_connections->next;
	}
	return (head);
}

void			mark_level_as_used(t_node *node_head)
{
	t_node		*node;

	node = node_head;
	while (node)
	{
		if (node->room->type != END)
			node->room->used = 2;
		node = node->next;
	}
}

static t_level	*create_level(t_level *previous, int depth, int flags)
{
	t_node		*parent_node;
	t_level		*new;
	t_path		*previous_connections;
	t_node		*head;

	if (!(new = (t_level *)malloc(sizeof(t_level) * 1)))
		handle_error(flags, "Malloc error");
	new->next = NULL;
	parent_node = previous->nodes;
	head = NULL;
	new->end_counter = 0;
	while (parent_node)
	{
		head = add_children(parent_node, head, &new->end_counter, flags);
		parent_node = parent_node->next;
	}
	new->nodes = head;
	new->depth = depth;
	mark_level_as_used(head);
	if (head == NULL)
	{
		free(new);
		new = NULL;
	}
	return (new);
}

void			mark_flows(t_room *to, t_room *from)
{
	t_path		*path;

	path = to->paths;
	while (((t_room *)path->room)->id != from->id)
		path = path->next;
	if (path->flow != UNUSED)
		path->flow = 0;
	else
		path->flow = -1;
	path = from->paths;
	while (((t_room *)path->room)->id != to->id)
		path = path->next;
	if (path->flow != UNUSED)
		path->flow = 0;
	else
		path->flow = 1;
}

static int		set_augmenting_path(t_level *level, t_room *start)
{
	t_node		*node;

	node = level->nodes;
	while (node)
	{
		if (node->room->type == END)
		{
			while (node)
			{
				mark_flows(node->room, node->prev_room);
				node = node->previous;
			}
			return (1);
		}
		node = node->next;
	}
	return (0);
}

static void		delete_levels(t_level *network, t_room *room_list)
{
	t_level		*next;
	t_node		*node;
	t_node		*node_next;
	t_room		*current;

	while (network)
	{
		next = network->next;
		node = network->nodes;
		while (node)
		{
			node_next = node->next;
			free(node);
			node = node_next;
		}
		free(network);
		network = next;
	}
	current = room_list;
	while (current)
	{
		current->used = 0;
		current = current->next;
	}
}

/*
** 1. Creates a node for each room from start of which flow is still unused.
**    (create_starting_level)
** 2. Creates new BFS levels until one or more node reaches end. (create_level)
**    When one does, end counter is increased. This function can move through
**    paths that have either flow UNUSED or -1, and to rooms of which used2 is
**    not marked yet.
** 3. Once end counter is above zero, we find one of the nodes that reached end
**    and move backwards in the tree (set_augmenting_path). On the way back we
**    set flows (mark_flows): If the path we move through was UNUSED, we set the
**    flow to be 1 towards end and -1 towards start. If the flows have been
**    previously marked, we set them to 0.
** 4. The function stops once end is found or no new connections can be made (if
**    all available path flows are either 1 or 0). Returns 1 if an augmenting
**    path was found and 0 otherwise.
*/

int				edmonds_karp_traverse(t_farm farm, int flags)
{
	t_level		*network;
	t_level		*network_head;
	int			path_found;
	int			depth;

	path_found = 0;
	farm.start->used = 2;
	network = create_starting_level(farm.start, flags);
	network_head = network;
	depth = 2;
	while (network)
	{
		if (network->end_counter)
		{
			path_found = set_augmenting_path(network, farm.start);
			break ;
		}
		network->next = create_level(network, depth, flags);
		network = network->next;
		depth++;
	}
	delete_levels(network_head, farm.rooms);
	return (path_found);
}
