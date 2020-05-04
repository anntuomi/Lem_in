#include "lemin.h"

static void		assign_paths(t_route **ants, t_routes **routes, int path_count,
int amount)
{
	int			moves;
	int			i1;
	int			i2;

	moves = calculate_moves(routes, path_count, amount);
	i1 = 0;
	i2 = 0;
	while (i1 < amount)
	{
		if (i2 == path_count || routes[i2]->rooms - 1 > moves)
		{
			i2 = 0;
			moves--;
		}
		ants[i1++] = routes[i2++]->route;
	}
	ants[i1] = NULL;
}

static char		*prefix_char(char *str, char *prefix, int prefix_len)
{
	char		*result;
	int			i;
	int			j;

	result = (char *)malloc(sizeof(char) * ft_strlen(str) + prefix_len + 1);
	i = 0;
	while (i < prefix_len)
	{
		result[i] = prefix[i];
		i++;
	}
	j = 0;
	while (str[j])
	{
		result[i] = str[j];
		i++;
		j++;
	}
	result[i] = '\0';
	return (result);
}

char			*add_to_command_line(char *line, char *nbr, char *name,
int *first)
{
	char		*block;
	char		*number_prefix;
	char		*name_prefix;
	char		*result;

	if (*first == 0)
		number_prefix = prefix_char(nbr, " L", 2);
	else
	{
		number_prefix = prefix_char(nbr, "L", 1);
		*first = 0;
	}
	free(nbr);
	name_prefix = prefix_char(name, "-", 1);
	block = ft_strjoin(number_prefix, name_prefix);
	free(number_prefix);
	free(name_prefix);
	result = ft_strjoin(line, block);
	if (line != NULL)
		free(line);
	free(block);
	return (result);
}

static char		*move_ants(t_route **ants)
{
	int			first;
	int			i;
	int			len;
	char		*line;

	len = 0;
	first = 1;
	i = 0;
	line = NULL;
	while (ants[i])
	{
		if (ants[i]->room->type != END &&
		(!ants[i]->next->room->ant_count
		|| ants[i]->next->room->type == END))
		{
			ants[i]->room->ant_count--;
			ants[i] = ants[i]->next;
			ants[i]->room->ant_count++;
			line = add_to_command_line(line, ft_itoa(i + 1),
			ants[i]->room->name, &first);
			//if (!first)
			//	printf(" ");
			//else
			//	first = 0;
			//printf("L%d-%s", i + 1, farm.ants[i]->room->name);
		}
		i++;
	}
	return (line);
}

/*int			find_longest_route(t_routes **used, int path_count)
{
	int i;
	int longest_route;

	i = 0;
	longest_route = used[0]->rooms - 2;
	while (i < path_count)
	{
		if (used[i]->rooms - 2 > longest_route)
			longest_route = used[i]->rooms - 2;
		i++;
	}
	return (longest_route);
}*/

int				calculate_moves(t_routes **routes, int path_count,
int ant_amount)
{
	int			moves;
	int			rooms;
	int			i;

	i = 0;
	rooms = 0;
	while (i < path_count)
		rooms += routes[i++]->rooms - 2;
	moves = ant_amount + rooms;
	moves = !(moves % path_count) ? moves / path_count : moves / path_count + 1;
	return (moves);
}

/*int				adjust_to_ant_amount(int path_count, int ant_amount,
t_routes **ordered, t_routes **used)
{
	int			moves;
	int			new_path_count;
	int			tested_moves;

	if (ordered[0]->rooms == 2)
		return (-1);
	new_path_count = path_count;
	moves = calculate_moves(used, path_count, ant_amount);
	while (path_count > 0)
	{
		if ((tested_moves = calculate_moves(used, path_count, \
		ant_amount)) < moves)
		{
			moves = tested_moves;
			new_path_count = path_count;
		}
		//printf("Path count: %d Moves: %f ", path_count, tested_moves);
		path_count--;
	}
	if (calculate_moves(ordered, 1, ant_amount) < moves)
		return (-1);
	return (new_path_count);
}*/

void			solve(t_farm farm, t_routes **ordered, int path_count)
{
	t_routes	**used_routes;
	//int			moves;
	char		*output;
	char		*line;
	char		*tmp;
	int			len;

	output = NULL;
	line = NULL;
	used_routes = determine_used_routes(farm, &path_count);
	order_routes(used_routes);
	//print_unique_routes(used_routes);
	//printf("\n");
	assign_paths(farm.ants, used_routes, path_count, farm.amount);
	//moves = 0;
	len = 0;
	while (farm.end->ant_count != farm.amount)
	{
		line = move_ants(farm.ants);
		tmp = ft_strjoin_new(output, line, &len, '\n');
		free(output);
		output = tmp;
		//moves++;
		free(line);
	}
	write(1, output, len);
	free(output);
	//printf("Moves: %d\n", moves);
}
