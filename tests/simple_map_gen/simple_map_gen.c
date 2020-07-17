/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_map_gen.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atuomine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/01 14:03:36 by atuomine          #+#    #+#             */
/*   Updated: 2020/06/01 14:03:37 by atuomine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "../../libft/includes/ft_printf.h"
#include "../../libft/includes/libft.h"

static void	create_links(int block_size, int room_count)
{
	int link1;
	int link2;
	int block_nbr;
	int block_limit;

	link1 = 2;
	link2 = 3;
	block_nbr = 0;
	block_limit = block_size;
	while (link2 <= room_count)
	{
		ft_printf("1-%d\n", link1);
		while (link2 <= block_limit && link2 < room_count)
		{
			ft_printf("%d-%d\n", link1, link2);
			link1++;
			link2++;
		}
		ft_printf("%d-%d\n", link2 - 1, room_count);
		block_limit = block_limit + block_size;
		link1++;
		link2++;
	}
}

static void	create_rooms(int path_count, int room_count, int ant_amount,
int block_size)
{
	int x;
	int y;
	int room_name;
	int i;

	i = 0;
	y = 1;
	room_name = 2;
	ft_printf("%d\n##start\n1 0 0\n", ant_amount);
	while (i < path_count || room_name < room_count)
	{
		x = 1;
		while (x <= block_size && room_name < room_count)
		{
			ft_printf("%d %d %d\n", room_name++, x, y);
			x++;
		}
		y++;
		i++;
	}
	ft_printf("##end\n%d %d %d\n", room_name, block_size + 1, y);
}

static void	print_error(void)
{
	ft_printf("Program needs 1+ ants, 3+ rooms and 1+ paths.\n");
	ft_printf("Arguments need to be positive integers.\n");
	exit(2);
}

static void	set_limits(int *path_count, int *ant_count, int *room_count)
{
	if (*room_count > 4000)
	{
		ft_printf("# Room count too big - count set to 4000\n");
		*room_count = 4000;
	}
	if (*ant_count > 1000)
	{
		ft_printf("# Ant count too big - count set to 1000\n");
		*ant_count = 1000;
	}
	if (*path_count > 2000)
	{
		ft_printf("# Path count too big - count set to 2000\n");
		*path_count = 2000;
	}
	if (*path_count > *room_count - 2)
	{
		ft_printf("# Path count too big - count set to room count - 2\n");
		*path_count = *room_count - 2;
	}
}

int			main(int argc, char **argv)
{
	int		path_count;
	int		ant_amount;
	int		room_count;
	int		block_size;

	if (argc < 4)
	{
		ft_printf("Usage: ./gen [ant amount] [room count] [path count]\n");
		exit(1);
	}
	path_count = ft_atoi(argv[3]);
	room_count = ft_atoi(argv[2]);
	ant_amount = ft_atoi(argv[1]);
	if (room_count < 3 || ant_amount < 1 || path_count < 1)
		print_error();
	set_limits(&path_count, &ant_amount, &room_count);
	block_size = room_count / path_count;
	create_rooms(path_count, room_count, ant_amount, block_size);
	create_links(block_size, room_count);
	return (0);
}
