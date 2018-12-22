/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opus1io <opus1io@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 18:28:54 by jplevy            #+#    #+#             */
/*   Updated: 2018/12/22 12:43:00 by opus1io          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

# include <libft.h>

# define USAGE "usage: ft_nm [-options | binary file]\n"
# define NB_FLAG 2
# define NB_ERROR 3

typedef enum	e_flags{
	NONE = 0,
	ALPHA = 1,
	REVERSE = 2,
	ST_ORDER = 4,
}				t_flags;

typedef struct	s_flag_funcs {
	char		flag;
	int			(*func_call)(t_flags *);
}				t_flag_funcs;

int				ft_r_flag(t_flags *f);
int				ft_p_flag(t_flags *f);

static const t_flag_funcs g_flag_calls[NB_FLAG] = {
	{'r', &ft_r_flag},
	{'p', &ft_p_flag},
};

static const char		*g_error_table[NB_ERROR] = {
	"Invalid arg : ",
	"Following args cannot be called twice : "
};
#endif
