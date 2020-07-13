/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in_checker.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmetelin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/13 14:03:36 by jmetelin          #+#    #+#             */
/*   Updated: 2020/07/13 14:03:37 by jmetelin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEM_IN_CHECKER_H
# define LEM_IN_CHECKER_H

# include "../libft/includes/libft.h"
# include "../libft/includes/get_next_line.h"
# include <stdlib.h>
# include <stdio.h>

# define START 0
# define NORMAL 1
# define END 2

typedef struct	s_path
{
	void			*room;
	struct s_path	*next;
}				t_path;

typedef struct	s_room
{
	int				type;
	char			*name;
	int				x;
	int				y;
	int				id;
	int				ant_count;
	t_path			*paths;
	struct s_room	*next;
}				t_room;

typedef struct	s_ant
{
	t_room	*room;
	int		mvd;
}				t_ant;

typedef struct	s_farm
{
	int		ant_count;
	t_ant	**ants;
	t_room	*rooms;
	t_room	*start;
	t_room	*end;
}				t_farm;

void			handle_error(char *str);
t_room			*get_rooms(char **line);
int				count_words(char **array);
int				is_nbr(char	*str);
void			set_links(char *line, t_room *room);
void			del_array(char **array);

#endif
