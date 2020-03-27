#include "libft.h"

int	ft_isnum(char *str)
{
	int	i;

	i = (str[0] == '-' ? 1 : 0);
	while (str[i])
	{
		if (ft_isdigit(str[i]) != 1)
			return (-1);
		i++;
	}
	return (1);
}
