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

static char		*prefix_char(char *str, char *prefix, int prefix_len)
{
	char		*result;
	int			i;
	int			j;

	if (!(result = (char *)malloc(sizeof(char) *
	(ft_strlen(str) + prefix_len + 1))))
		handle_error();
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
int *first)
{
	char		*result;
	char		*block;
	char		*number_prefix;
	char		*name_prefix;

	if (!*first)
		number_prefix = prefix_char(nbr, " L", 2);
	else
	{
		number_prefix = prefix_char(nbr, "L", 1);
		*first = 0;
	}
	free(nbr);
	name_prefix = prefix_char(name, "-", 1);
	if (!(block = ft_strjoin(number_prefix, name_prefix)))
		handle_error();
	free(number_prefix);
	free(name_prefix);
	if (!(result = ft_strjoin(line, block)))
		handle_error();
	if (line)
		free(line);
	free(block);
	return (result);
}

char			*move_ants(t_fork **ants)
{
	int			first;
	int			i;
	char		*line;
	char		*nbr;

	first = 1;
	i = 0;
	line = NULL;
	while (ants[i])
	{
		if (ants[i]->from->type != END &&
		(!ants[i]->next->from->ant_count || ants[i]->next->from->type == END))
		{
			ants[i]->from->ant_count--;
			ants[i] = ants[i]->next;
			ants[i]->from->ant_count++;
			if (!(nbr = ft_itoa(i + 1)))
				handle_error();
			line = add_to_command_line(line, nbr, ants[i]->from->name, &first);
		}
		i++;
	}
	return (line);
}
