/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_ants.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuomine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/01 14:03:43 by atuomine          #+#    #+#             */
/*   Updated: 2020/06/01 14:03:46 by atuomine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static char		*prefix_char(char *str, char *prefix, int prefix_len, int flags)
{
	char		*result;
	int			i;
	int			j;

	if (!(result = (char *)malloc(sizeof(char) *
	(ft_strlen(str) + prefix_len + 1))))
		handle_error(flags, "Malloc error");
	i = 0;
	while (i < prefix_len)
	{
		result[i] = prefix[i];
		i++;
	}
	j = 0;
	while (str[j])
		result[i++] = str[j++];
	result[i] = '\0';
	return (result);
}

static char		*add_to_command_line(char *line, char *nbr, char *name,
int *first, int flags)
{
	char		*result;
	char		*block;
	char		*number_prefix;
	char		*name_prefix;

	if (!*first)
		number_prefix = prefix_char(nbr, " L", 2, flags);
	else
	{
		number_prefix = prefix_char(nbr, "L", 1, flags);
		*first = 0;
	}
	free(nbr);
	name_prefix = prefix_char(name, "-", 1, flags);
	if (!(block = ft_strjoin(number_prefix, name_prefix)))
		handle_error(flags, "Malloc error");
	free(number_prefix);
	free(name_prefix);
	if (!(result = ft_strjoin(line, block)))
		handle_error(flags, "Malloc error");
	if (line)
		free(line);
	free(block);
	return (result);
}

char			*move_ants(t_ant **ants, int flags)
{
	int			first;
	int			i;
	char		*line;
	char		*nbr;
	t_room		*room;
	int			fork;

	first = 1;
	i = 0;
	line = NULL;
	while (ants[i])
	{
		fork = 0;
		if (ants[i]->room->type != END)
		{
			if (!ants[i]->fork || ants[i]->room->id != ants[i]->fork->from->id)
			{
				if (!ants[i]->prev ||
				((t_room *)ants[i]->room->paths->room)->id != ants[i]->prev->id)
					room = ants[i]->room->paths->room;
				else
					room = ants[i]->room->paths->next->room;
			}
			else
			{
				room = ants[i]->fork->to;
				fork = 1;
			}
			if (!room->ant_count || room->type == END)
			{
				ants[i]->room->ant_count--;
				ants[i]->prev = ants[i]->room;
				ants[i]->room = room;
				ants[i]->room->ant_count++;
				if (fork)
					ants[i]->fork = ants[i]->fork->next;
				if (!(nbr = ft_itoa(i + 1)))
					handle_error(flags, "Malloc error");
				line = add_to_command_line(line, nbr, ants[i]->room->name,
				&first, flags);
			}
		}
		i++;
	}
	return (line);
}
