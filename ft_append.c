#include "lemin.h"

static int	ft_joins1(char const *s1, char *new)
{
	int i;

	i = 0;
	if (s1)
	{
		while (s1[i])
		{
			new[i] = s1[i];
			i++;
		}
	}
	return (i);
}

static char	*add_end(char *new, int i, char c)
{
	if (c != 0)
	{
		new[i] = c;
		i++;
	}
	new[i] = '\0';
	return (new);
}

char		*ft_append(char const *s1, char const *s2, int *len, char c)
{
	char	*new;
	int		length;
	int		i;
	int		start;

	length = *len + ft_strlen(s2);
	if (c != 0)
		length = length + 1;
	if ((new = ft_strnew(length)) == NULL)
		handle_error();
	start = 0;
	i = ft_joins1(s1, new);
	if (s2)
	{
		while (s2[start])
		{
			new[i] = s2[start];
			i++;
			start++;
		}
	}
	new = add_end(new, i, c);
	*len = length;
	return (new);
}
