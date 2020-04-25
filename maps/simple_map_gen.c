#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	main(int argc, char **argv)
{
	if (argc != 4)
	{
		printf("Usage: ./gen [ant amount] [room count] [path count] > new_file_name.\n");
		exit(0);
	}
	int	room_name = 2;
	int x = 1;
	int y = 1;
	int	path_count = atoi(argv[3]);
	int room_count = atoi(argv[2]);
	int ant_amount = atoi(argv[1]);
	int block_size = (room_count) / path_count;
	int	last_rooms = (room_count) % path_count;
	printf("%d\n##start\n1 0 0\n", ant_amount);
	for (int i = 0; i < path_count; i++)
	{
		for (x = 1; x <= block_size && room_name < room_count; x++)
		{
			printf("%d %d %d\n", room_name++, x, y);
		}
		y++;
	}
	printf("##end\n%d %d %d\n", room_name, block_size + 1, y);
	int link1 = 2;
	int link2 = 3;
	int block_nbr = 0;
	int block_limit = block_size + 1;
	while (link2 <= room_count)
	{
		printf("1-%d\n", link1);
		while (link2 <= block_limit && link2 < room_count)
		{
			printf("%d-%d\n", link1, link2);
			link1++;
			link2++;
		}
		printf("%d-%d\n", link2 - 1, room_count);
		block_limit = block_limit + block_size;
		link1++;
		link2++;
	}
}