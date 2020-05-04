#include "lemin.h"

static char	*prefix_char(char *str, char *prefix, int prefix_len)
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

static char	*add_to_command_line(char *line, char *nbr, char *name,
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

char		*move_ants(t_route **ants)
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
