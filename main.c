/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
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

static void		del_room(t_room **head, t_room **room)
{
	t_room		*cur;
	t_room		*next;

	cur = *room;
	while (cur && cur->type == NORMAL && (!cur->paths || !cur->paths->next))
	{
		free(cur->name);
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
		free(cur);
		cur = next;
	}
	if (cur && cur->type != NORMAL && !cur->paths)
		handle_error();
}

static void		find_edges(t_room **head, t_room **start, t_room **end)
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
			handle_error();
		if (room->type != NORMAL && !room->paths)
			handle_error();
		else if (room->type == NORMAL && (!room->paths || !room->paths->next))
			del_room(head, &room);
		else
			room = room->next;
	}
	if (!*start || !*end)
		handle_error();
}

static t_fork	**get_ants(int *ant_count, t_input **input)
{
	t_fork		**ants;
	char		*line;
	long long	tmp;

	while (get_next_line(0, &line) == 1 && line[0] == '#')
	{
		if (line[1] != '#')
		{
			(*input)->line = line;
			if (!((*input)->next = (t_input *)malloc(sizeof(t_input))))
				handle_error();
			*input = (*input)->next;
		}
		else
			free(line);
	}
	if (!line || !ft_isnum(line) || (tmp = ft_atoll(line)) < 1 || tmp > INT_MAX)
		handle_error();
	*ant_count = (int)tmp;
	(*input)->line = line;
	(*input)->next = NULL;
	if (!(ants = (t_fork **)malloc(sizeof(t_fork *) * (*ant_count + 1))))
		handle_error();
	return (ants);
}

static void		print_branches(t_branch **branch)
{
	t_route		**route;
	t_fork		*fork;
	int			i;
	int			ii;
	int			iii;

	i = 0;
	while (branch[i])
	{
		printf("%d. Branch (%d route(s))\n", i + 1, branch[i]->routes);
		route = branch[i]->array;
		ii = 0;
		while (route[ii])
		{
			printf("%d.%d Route (%d room(s))\n", i + 1, ii + 1,
			route[ii]->rooms);
			fork = route[ii]->forks;
			iii = 1;
			while (fork)
			{
				printf("%d.%d.%d Fork (from %s to %s)\n", i + 1, ii + 1, iii++,
				fork->from->name, fork->to->name);
				fork = fork->next;
			}
			if (route[++ii])
				printf("\n");
		}
		if (branch[++i])
			printf("\n\n");
	}
}

static void		print_rooms(t_room *room)
{
	t_path		*path;
	t_room		*tmp;
	int			i;

	i = 1;
	while (room)
	{
		printf("%d. %s\n", i++, room->name);
		path = room->paths;
		while (path)
		{
			tmp = path->room;
			printf("- %s\n", tmp->name);
			path = path->next;
		}
		room = room->next;
	}
}

void			print_routes(t_route **routes, t_room *start, t_room *end)
{
	t_fork *current;
	int i;

	i = 0;
	printf("Start room %s, end room %s\n", start->name, end->name);
	while (routes[i])
	{
		current = routes[i]->forks;
		printf("Route nbr %d len %d\n", i, routes[i]->rooms);
		while (current)
		{
			printf("Depth %d: From %s to %s\n", current->from->depth, current->from->name, current->to->name);
			current = current->next;
		}
		i++;
	}
}

typedef struct		s_group
{
	t_route			**routes;
	int				path_count;
	int				moves;
	struct s_group	*next;

}					t_group;

int			count_group_size(t_room *start)
{
	int size;
	t_path *current;
	int ref;

	size = 0;
	current = start->paths;
	if (start->type == END)
		ref = -1;
	else
		ref = 1;	
	while (current)
	{
		if (current->flow == ref)
			size++;
		current = current->next;
	}
	return (size);
}

t_fork			*create_node(t_room *current)
{
	t_fork *fork;
		
	fork = (t_fork *)malloc(sizeof(t_fork));
	fork->from = current;
	fork->to = current;
	fork->next = NULL;
	return (fork);
}
void			count_ones(t_path *path, int group_nbr)
{
	t_path *current;
	int count = 0;
	int visited = 0;

	current = path;
	while (current)
	{
		if (current->flow == 1)
		{
			count++;
			if (((t_room *)current->room)->visited == group_nbr)
				visited++;
		}
		current = current->next;
	}
	if (count > 1)
	{
		printf("Found %d flows of 1 of which visited %d\n", count, visited);
	}
}
t_fork			*save_r(t_room *start, t_room *current, int *length, int group_nbr)
{
	t_fork *fork_head;
	t_fork *fork;
	t_path	*path;
	int		depth;
	int 	reroute_count = 0;

	fork = (t_fork *)malloc(sizeof(t_fork));
	fork->from = start;
	fork->to = current;
	fork->from->depth = 0;
	fork->to->depth = 1;
	fork->next = NULL;
	fork_head = fork;
	path = current->paths;
	*length = 2;
	current->visited = group_nbr;
	depth = 2;
	while (current->type != END)
	{
		path = current->paths;
		current->depth = depth;
		count_ones(path, group_nbr);
		while (path && (path->flow != 1 || ((t_room *)path->room)->visited == group_nbr))
		{
			/*if (((t_room *)path->room)->visited == group_nbr && path->flow == 1)
			{
				if (((t_room *)path->room)->depth > current->depth + 1)
				{
					reroute_count++;
					//break ;
				}
				printf("Visited depth %d, current depth %d\n", ((t_room *)path->room)->depth, current->depth);
			}*/
			path = path->next;
		}
		if (path == NULL)
		{
			*length = 0;
			free(fork);
			return (NULL);
		}
		current = ((t_room *)path->room);
		if (current->type != END)
			current->visited = group_nbr;
		fork->next = create_node(current);
		*length = *length + 1;
		fork = fork->next;
		depth++;
	}
	fork->next = NULL;
	group_nbr++;
	printf("Reroute count %d\n", reroute_count);
	return (fork_head);
}

int				calculate_moves(t_route **routes, int path_count,
int ant_count)
{
	int			moves;
	int			rooms;
	int			i;

	rooms = 0;
	i = 0;
	while (i < path_count)
		rooms += routes[i++]->rooms - 2;
	moves = (ant_count + rooms) / path_count +
	((ant_count + rooms) % path_count ? 1 : 0);
	return (moves);
}

t_group			*rebuild_routes(t_farm farm, int group_nbr)
{
	t_group *group;
	int		group_size;
	t_route	**routes;
	int		i;
	int		length;
	t_path	*current;

	group = (t_group *)malloc(sizeof(t_group));
	group_size = count_group_size(farm.start);
	routes = (t_route **)malloc(sizeof(t_route *) * group_size + 1);
	routes[group_size] = NULL;
	i = 0;
	current = farm.start->paths;
	while (i < group_size)
	{
		if (current->flow == 1)
		{
			routes[i] = (t_route *)malloc(sizeof(t_route));
			routes[i]->forks = save_r(farm.start, ((t_room *)current->room), &length, group_nbr);
			routes[i]->rooms = length;
			if (length == 0)
			{
				group_size--;
			}
			else
				i++;
		}
		current = current->next;
	}
	routes[i] = NULL;
	order_routes_simple(routes);
	group->routes = routes;
	group->path_count = group_size;
	if (group->path_count > 0)
		group->moves = calculate_moves(group->routes, group->path_count, farm.ant_count);
	else
		group->moves = -1;
	group->next = NULL;
	return (group);
}	

static void		assign_paths(t_fork **ants, int ant_count, t_route **routes,
int path_count)
{
	int			moves;
	int			i;
	int			j;

	moves = calculate_moves(routes, path_count, ant_count);
	i = 0;
	j = 0;
	while (i < ant_count)
	{
		if (j == path_count || routes[j]->rooms - 1 > moves)
		{
			j = 0;
			moves--;
		}
		ants[i++] = routes[j++]->forks;
	}
	ants[i] = NULL;
	free(routes);
}

int				main(void)
{
	t_input		*head;
	t_input		*input;
	t_farm		farm;
	char		*line;

	if (!(head = (t_input *)malloc(sizeof(t_input))))
		handle_error();
	input = head;
	farm.ants = get_ants(&farm.ant_count, &input);
	farm.rooms = NULL;
	create_room_list(&farm.rooms, &line, &input);
	if (!line)
		handle_error();
	set_links(line, farm.rooms, &input);
	//print_rooms(farm.rooms);
	find_edges(&farm.rooms, &farm.start, &farm.end);
	t_route *shortest;
	t_group *groups;
	t_group *best;

	int group_nbr = 1;

	shortest = find_shortest_route(farm);
	if (shortest)
	{
		groups = rebuild_routes(farm, group_nbr);
		best = groups;
		//print_routes(groups->routes, farm.start, farm.end);
		groups = groups->next;
		group_nbr++;
	}
	while (shortest)
	{
		shortest = find_shortest_route(farm);
		if (shortest)
		{
			groups = rebuild_routes(farm, group_nbr);
			if (best->moves == -1 || (groups->moves != -1 && groups->moves < best->moves))
				best = groups;
			//print_routes(groups->routes, farm.start, farm.end);
			groups = groups->next;
		}
		group_nbr++;
	}
	printf("Best group moves %d\n", best->moves);
	exit(1);
	assign_paths(farm.ants, farm.ant_count, best->routes, best->path_count);
	printf("Best group moves %d\n", best->moves);
	int moves;
	char *tmp;
	int len;
	char *output;

	output = NULL;
	len = 0;
	moves = 0;
	while (farm.end->ant_count != farm.ant_count)
	{
		line = move_ants(farm.ants);
		tmp = ft_append(output, line, &len, '\n');
		if (output)
			free(output);
		output = tmp;
		free(line);
		if (len > 1000 || farm.end->ant_count == farm.ant_count)
			print_output(&output, &len);
		moves++;
	}
	char *result = ft_itoa(moves);
	write(1, result, strlen(result));
	printf(" Real moves %d\n", best->moves);
	//print_rooms(farm.rooms);
	//farm.branches = get_branches_to_end(farm.start);
	//print_input(head);
	//farm.branch_count = count_branches(farm.branches);
	//farm.ordered = branches_to_array(farm.branch_count, farm.branches, NULL);
	//order_routes(farm.ordered, NULL);
	//print_branches(farm.ordered);
	/*farm.start->ant_count = farm.ant_count;
	farm.path_count = count_max_path_count(farm.routes, farm.ordered[0]);
	solve(farm);
	free_memory(farm);*/
	return (0);
}
