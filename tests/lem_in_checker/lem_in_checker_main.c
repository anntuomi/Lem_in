/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetelin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/13 14:03:36 by jmetelin          #+#    #+#             */
/*   Updated: 2020/07/13 14:03:37 by jmetelin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in_checker.h"

static void		mv_ant(char *mv, int ant_count, t_ant **ants)
{
	char	**array;
	int		ant;
	char	*room;
	char	*nbr;
	t_path	*path;

	if (!(array = ft_strsplit(mv, '-')) || count_words(array) != 2 ||
	array[0][0] != 'L' || !is_nbr((nbr = array[0] + 1)))
		handle_error("Malloc error or the results do not follow the format");
	if ((ant = ft_atoi(nbr)) < 1 || ant > ant_count || ants[--ant]->mvd)
		handle_error("An undefined or already moved ant is moved");
	room = array[1];
	path = ants[ant]->room->paths;
	while (path && !ft_strequ(((t_room *)path->room)->name, room))
		path = path->next;
	del_array(array);
	if (!path || (((t_room *)path->room)->type == NORMAL &&
	((t_room *)path->room)->ant_count))
		handle_error("An ant is moved to an undefined or a full room");
	ants[ant]->room->ant_count--;
	ants[ant]->room = path->room;
	ants[ant]->room->ant_count++;
	ants[ant]->mvd = 1;
}

static void		mv_ants(int ant_count, t_ant **ants, t_room *start, t_room *end)
{
	char	*line;
	char	**array;
	int		i;

	i = 0;
	while (ants[i])
		ants[i++]->room = start;
	start->ant_count = ant_count;
	while (get_next_line(0, &line) == 1)
	{
		if (!(array = ft_strsplit(line, ' ')))
			handle_error("Malloc error");
		i = 0;
		while (array[i])
			mv_ant(array[i++], ant_count, ants);
		del_array(array);
		ft_strdel(&line);
		i = 0;
		while (ants[i])
			ants[i++]->mvd = 0;
	}
	if (end->ant_count != ant_count)
		handle_error("All the ants are not brought to ##end");
}

static void		set_start_end(t_room *head, t_room **start, t_room **end)
{
	t_room	*room;

	room = head;
	*start = NULL;
	*end = NULL;
	while (room)
	{
		if (room->type == START && !*start)
			*start = room;
		else if (room->type == END && !*end)
			*end = room;
		else if (room->type != NORMAL)
			handle_error("Multiple ##start or ##end rooms");
		if (room->type != NORMAL && !room->paths)
			handle_error("No paths");
		room = room->next;
	}
	if (!*start || !*end)
		handle_error("No ##start or/and ##end");
}

static t_ant	**get_ants(int *ant_count)
{
	t_ant	**ants;
	char	*line;
	int		i;

	while (get_next_line(0, &line) == 1 && line[0] == '#')
		ft_strdel(&line);
	if (!line || (*ant_count = ft_atoi(line)) < 1 ||
	!(ants = (t_ant **)malloc(sizeof(t_ant *) * (*ant_count + 1))))
		handle_error("Number_of_ants is missing or invalid or malloc error");
	ft_strdel(&line);
	i = 0;
	while (i < *ant_count)
	{
		if (!(ants[i] = (t_ant *)malloc(sizeof(t_ant))))
			handle_error("Malloc error");
		ants[i]->room = NULL;
		ants[i++]->mvd = 0;
	}
	ants[i] = NULL;
	return (ants);
}

int				main(void)
{
	t_farm	farm;
	char	*line;

	farm.ants = get_ants(&farm.ant_count);
	farm.rooms = get_rooms(&line);
	set_links(line, farm.rooms);
	set_start_end(farm.rooms, &farm.start, &farm.end);
	mv_ants(farm.ant_count, farm.ants, farm.start, farm.end);
	ft_putstr("OK\n");
	return (0);
}
