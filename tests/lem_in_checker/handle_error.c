/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetelin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/13 14:03:36 by jmetelin          #+#    #+#             */
/*   Updated: 2020/07/13 14:03:37 by jmetelin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in_checker.h"

void	del_array(char **array)
{
	int	i;

	i = 0;
	if (array)
	{
		while (array[i])
		{
			free(array[i]);
			i++;
		}
		free(array);
		array = NULL;
	}
}

int		is_nbr(char *str)
{
	if (*str == '-')
		*str++;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		*str++;
	}
	return (1);
}

int		count_words(char **array)
{
	int	words;

	words = 0;
	while (array[words])
		words++;
	return (words);
}

void	handle_error(char *str)
{
	ft_putstr("KO: ");
	ft_putstr(str);
	ft_putstr("\n");
	exit(EXIT_FAILURE);
}
