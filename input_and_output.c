/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_and_output.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuomine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/01 14:03:18 by atuomine          #+#    #+#             */
/*   Updated: 2020/06/01 14:03:20 by atuomine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void			print_output(char **output, int *len)
{
	write(1, *output, *len);
	free(*output);
	*output = NULL;
	*len = 0;
}

void			print_input(t_input *input, int flags)
{
	char	*output;
	char	*tmp;
	int		len;
	t_input	*next;

	output = NULL;
	len = 0;
	while (input)
	{
		tmp = ft_append(output, input->line, &len, '\n', flags);
		if (output)
			free(output);
		output = tmp;
		if (input->line)
			free(input->line);
		next = input->next;
		free(input);
		input = next;
		if (len > 1000 || !input)
			print_output(&output, &len);
	}
	ft_putchar('\n');
}

void			set_input(t_input **input, char *line, int rooms, int flags)
{
	if (line[0] == '#' && line[1] == '#' &&
	((rooms && determine_room_type(line) == NORMAL) || !rooms))
	{
		free(line);
		line = NULL;
	}
	else
	{
		if (!((*input)->next = (t_input *)malloc(sizeof(t_input))))
			handle_error(flags, "Malloc error");
		*input = (*input)->next;
		(*input)->line = line;
		(*input)->next = NULL;
	}
}
