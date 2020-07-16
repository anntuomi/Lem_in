/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_to_command_line.c                              :+:      :+:    :+:   */
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
		handle_error(0, "Malloc error");
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

char			*add_to_command_line(char *line, t_block *block,
int *first)
{
	char	*result;
	char	*block_text;
	char	*number_prefix;
	char	*name_prefix;

	if (!*first)
		number_prefix = prefix_char(block->ant_nbr, " L", 2);
	else
	{
		number_prefix = prefix_char(block->ant_nbr, "L", 1);
		*first = 0;
	}
	free(block->ant_nbr);
	name_prefix = prefix_char(block->ant_room_name, "-", 1);
	if (!(block_text = ft_strjoin(number_prefix, name_prefix)))
		handle_error(0, "Malloc error");
	free(number_prefix);
	free(name_prefix);
	if (!(result = ft_strjoin(line, block_text)))
		handle_error(0, "Malloc error");
	if (line)
		free(line);
	free(block_text);
	free(block);
	return (result);
}
