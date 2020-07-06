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

t_level	*create_starting_level(t_room *start)
{
	t_node *current;
	t_node *head;
	t_path *current_connection;
	t_level *level;
	int		id;

	id = 1;
	level = (t_level *)malloc(sizeof(t_level) * 1);
	level->next = NULL;
	level->size = 0;
	head = NULL;
	current_connection = start->paths;
	while (current_connection)
	{
		if (current_connection->flow == UNUSED)
		{
			current = (t_node *)malloc(sizeof(t_node) * 1);
			current->room = ((t_room *)current_connection->room);
			current->room->used = 2;
			//current->room->depth = 1;
			current->prev_room = start;
			current->next = head;
			current->id = id;
			current->previous = NULL;
			current->path_count = 1;
			head = current;
			id++;
			level->size++;
		}
		current_connection = current_connection->next;
	}
	//start->depth = 0;
	level->nodes = head;
	//level->depth = 1;
	return (level);
}

void	block_flow(t_room *room, int id_of_blocked_room)
{
	t_path *current;

	current = room->paths;
	while (current)
	{
		if (((t_room *)current->room)->id == id_of_blocked_room)
			current->flow = 0;
		current = current->next;
	}
}

t_level *create_level(t_level *previous, int depth)
{
	t_node *previous_lvl;
	t_level *new;
	t_node *new_lvl;
	t_path *previous_connections;
	t_node *head;
	int emergency_path_counter;
	int	valid_path_counter;
	int id;

	new = (t_level *)malloc(sizeof(t_level) * 1);
	new->next = NULL;
	previous_lvl = previous->nodes;
	head = NULL;
	new->end_counter = 0;
	while (previous_lvl)
	{
		previous_connections = previous_lvl->room->paths;
		emergency_path_counter = 0;
		valid_path_counter = 0;
		id = previous_lvl->id;
		previous_lvl->path_count = 0;
		while (previous_connections && previous_lvl->room->type != END)
		{
			if (((t_room *)previous_connections->room)->used != 2 && previous_connections->flow == UNUSED) //|| ((t_room *)previous_connections->room)->depth == depth)
			{
				new_lvl = (t_node *)malloc(sizeof(t_node) * 1);
				new_lvl->room = ((t_room *)previous_connections->room);
				if (new_lvl->room->type != END)
					new_lvl->room->used = 2;
				if (new_lvl->room->type == END)
					new->end_counter++;
                //new_lvl->room->depth = depth;
				new_lvl->prev_room = previous_lvl->room;
				new_lvl->next = head;
				new_lvl->id = id;
				new_lvl->path_count = previous_lvl->path_count;
				new_lvl->previous = previous_lvl;
				head = new_lvl;
				valid_path_counter++;
			}
			else if (previous_connections->flow == -1)
				emergency_path_counter++;
			previous_connections = previous_connections->next;
		}
		if (valid_path_counter == 0 && emergency_path_counter > 0)
		{
			previous_connections = previous_lvl->room->paths;
			id = previous_lvl->id;
			previous_lvl->path_count = 0;
			while (previous_connections && previous_lvl->room->type != END)
			{
				if (((t_room *)previous_connections->room)->used != 2 && previous_connections->flow == -1) //|| ((t_room *)previous_connections->room)->depth == depth)
				{
					//previous_connections->flow = 0;
					new_lvl = (t_node *)malloc(sizeof(t_node) * 1);
					new_lvl->room = ((t_room *)previous_connections->room);
					//block_flow(new_lvl->room, previous_lvl->room->id);
					if (new_lvl->room->type != END)
						new_lvl->room->used = 2;
					if (new_lvl->room->type == END)
						new->end_counter++;
					//new_lvl->room->depth = depth;
					new_lvl->prev_room = previous_lvl->room;
					new_lvl->next = head;
					new_lvl->id = id;
					new_lvl->path_count = previous_lvl->path_count;
					new_lvl->previous = previous_lvl;
					head = new_lvl;
				}
				previous_connections = previous_connections->next;
			}
		}
		previous_lvl = previous_lvl->next;
	}
	new->end_capacity = (depth - 2) * new->end_counter;
	new->nodes = head;
	//new->depth = depth;
	if (head == NULL)
		new = NULL;
	return (new);
}

void	mark_flows(t_room *to, t_room *from)
{
	t_path *path;
	
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

t_fork *save_route(t_node *nodes_head, t_room *start)
{
	t_fork *fork;
	t_fork *fork_head;
	t_node *node;
	t_path *path;

	node = nodes_head;
	fork_head = NULL;
	while (node)
	{
		fork = (t_fork *)malloc(sizeof(t_fork));
		fork->to = node->room;
		fork->from = node->prev_room;
		mark_flows(fork->to, fork->from);
		fork->to->used = 1;
		fork->from->used = 1;
		fork->next = fork_head;
		fork_head = fork;
		node = node->previous;
	}
	return (fork_head);
}
t_route 	*testing(t_level *level, t_route *route_head, t_room *start)
{
	int i;
	t_node *current;
	t_route *route;
	t_fork *forks;

	current = level->nodes;
	i = 0;
	while (current)
	{
		if (current->room->type == END)
		{
			route = (t_route *)malloc(sizeof(t_route));
			route->rooms = level->depth;
			route->forks = save_route(current, start);
			route->next = route_head;
			route_head = route;
			i++;
		}
		current = current->next;
	}
	return (route_head);
}

void	mark_end_connections(t_room *end)
{
	t_path *current;

	current = end->paths;
	while (current)
	{
		((t_room *)current->room)->start_connection = 2;
		current = current->next;
	}
}

void	delete_levels(t_level *network, t_room *room_list)
{
	t_level *next;
	t_node *node;
	t_node *node_next;
	t_room *current;

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
		if (current->used == 2)
		{	
			current->used = 0;
			//current->depth = -1;
		}
		current = current->next;
	}
}

t_route	*find_shortest_route(t_farm farm)
{
	t_level *network;
	t_level *network_head;
	t_route	*route_head;
	int		i;

	route_head = NULL;
	farm.start->used = 1;
	network = create_starting_level(farm.start);
	if (network->size == 0)
		return NULL;
	network_head = network;
	mark_end_connections(farm.end);
	//network->depth = 2;
	i = 2;
	int end_counter;
	end_counter = 0;
	network_head = network;
	while (network && !end_counter)
	{
		network->next = create_level(network, i);
		network = network->next;
		if (!network)
			break ;
		if (network->end_counter)
		{
			end_counter = network->end_counter;
			route_head = testing(network, route_head, farm.start);
		}
		i++;
	}
	delete_levels(network_head, farm.rooms);
	return (route_head);
}



















/*
int		count_level_size(t_level **previous)
{
	t_path	*current;
	int		size;
	int		i;

	i = 0;
	size = 0;
	while (previous[i])
	{
		current = previous[i]->room->paths;
		while (current)
		{
			((t_room *)current->room)->start_connection = 1;
			size++;
			current = current->next;
		}
		i++;
	}
	return (size);
}

t_level **create_level_array(int level_size, t_level **previous, int depth)
{
	int i;
	int j;
	t_level **level;
	t_path *current;
	int id;

	printf("Level size in creation %d\n", level_size);
	level = (t_level **)malloc(sizeof(t_level *) * level_size + 1);
	j = 0;
	i = 0;
	while (previous[i])
	{
		printf("Previous room name %s\n", previous[i]->room->name);
		current = previous[i]->room->paths;
		id = (depth != 1 ? i + 1 : j + 1);
		while (current)
		{
			level[j] = (t_level *)malloc(sizeof(t_level) * 1);
			level[j]->previous = previous[i]->room;
			level[j]->room = ((t_room *)current->room);
			level[j]->room->used = id;
			j++;
			current = current->next;
		}
		i++;
	}
	level[j] = NULL;
	return (level);
}

void	make_branches(t_farm farm)
{
	t_graph *graph_head;
	t_graph *current;
	int		level_size;

	graph_head = (t_graph *)malloc(sizeof(t_graph) * 1);
	graph_head->level = (t_level **)malloc(sizeof(t_level *) * 2);
	graph_head->next = NULL;
	graph_head->depth = 0;
	graph_head->level[0] = (t_level *)malloc(sizeof(t_level) * 1);
	graph_head->level[0]->room = farm.start;
	graph_head->level[0]->lvl_index = -1;
	graph_head->level[0]->previous = NULL;
	graph_head->level[1] = NULL;
	level_size = count_level_size(graph_head->level);
	printf("Next level size %d\n", level_size);
	current = graph_head->next;

	current = (t_graph *)malloc(sizeof(t_graph) * 1);
	current->depth = 1;
	current->level = create_level_array(level_size, graph_head->level, 1);
	current->next = NULL;
	level_size = count_level_size(current->level);
	printf("Next level size %d\n", level_size);
	exit(1);
}*/