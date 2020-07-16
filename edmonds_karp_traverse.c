/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edmonds_karp_traverse.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuomine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/16 11:26:37 by atuomine          #+#    #+#             */
/*   Updated: 2020/06/16 11:26:53 by atuomine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

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
**    paths that have either flow UNUSED or -1, and to rooms of which visited
**    is not marked yet.
** 3. Once end counter is above zero, we find one of the nodes that reached end
**    and move backwards in the tree (set_augmenting_path). On the way back we
**    set flows (mark_flows): If the path we move through was UNUSED, we set
**    the flow to be 1 towards end and -1 towards start. If the flows have been
**    previously marked, we set them to 0.
** 4. The function stops once end is found or no new connections can be made
**    (if all available path flows are either 1 or 0). Returns 1 if an
**    augmenting path was found and 0 otherwise.
*/

int				edmonds_karp_traverse(t_farm farm)
{
	t_level		*network;
	t_level		*network_head;
	int			path_found;

	path_found = 0;
	farm.start->used = 1;
	network = create_starting_level(farm.start);
	if (!network->nodes)
	{
		free(network);
		network = NULL;
	}
	network_head = network;
	while (network)
	{
		if (network->end_counter)
		{
			path_found = set_augmenting_path(network, farm.start);
			break ;
		}
		network->next = create_level(network);
		network = network->next;
	}
	delete_levels(network_head, farm.rooms);
	return (path_found);
}
