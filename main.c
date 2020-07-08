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

static t_ant	**get_ants(int *ant_count, t_input **input)
{
	t_ant		**ants;
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
	if (!(ants = (t_ant **)malloc(sizeof(t_ant *) * (*ant_count + 1))))
		handle_error();
	return (ants);
}

/*
** Route_cmp and are_unique_routes functions are only used in testing
*/

static int		route_cmp(t_fork *route1, t_fork *head)
{
	t_fork		*route2;

	while (route1)
	{
		route2 = head;
		while (route2)
		{
			if (route1->from->type != START && route2->from->type != START &&
			route2->from->id == route1->from->id)
				return (1);
			route2 = route2->next;
		}
		route1 = route1->next;
	}
	return (0);
}

static int		are_unique_routes(t_route **route)
{
	t_fork		*fork;
	int			i;
	int			j;

	i = 0;
	while (route[i])
	{
		j = i + 1;
		while (route[j])
		{
			if (route_cmp(route[i]->forks, route[j]->forks))
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

static void		print_routes(t_route **route)
{
	t_fork		*fork;
	int			i;
	int			j;

	i = 0;
	while (route[i])
	{
		printf("%d. Route %d (%d room(s))\n", i + 1, route[i]->id,
		route[i]->rooms);
		fork = route[i]->forks;
		j = 1;
		while (fork)
		{
			printf("%d.%d Fork (from %s to %s)\n", i + 1, j++,
			fork->from->name, fork->to->name);
			fork = fork->next;
		}
		if (route[++i])
			printf("\n");
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
	find_edges(&farm.rooms, &farm.start, &farm.end);
	//print_rooms(farm.rooms);
	farm.branches = get_branches_to_end(farm.start);
	//print_input(head);
	farm.route_count = count_routes(farm.branches);
	farm.ordered = routes_to_array(farm.route_count, farm.branches);
	order_routes(farm.ordered);
	//print_routes(farm.ordered);
	//printf("unique %d\n", are_unique_routes(farm.ordered));
	farm.start->ant_count = farm.ant_count;
	//farm.path_count = count_max_path_count(farm.routes, farm.ordered[0]);
	solve(farm);
	//free_memory(farm);
	return (0);
}
