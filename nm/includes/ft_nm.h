/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opus1io <opus1io@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 18:28:54 by jplevy            #+#    #+#             */
/*   Updated: 2019/01/16 12:53:46 by opus1io          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

# include <libft.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/mman.h>

# include <mach-o/loader.h>
# include <mach-o/nlist.h>

# define USAGE "usage: ft_nm [-options | binary file]\n"
# define NB_FLAG 2
# define NB_ERROR 3

typedef enum	e_flags
{
	NONE = 0,
	ALPHA = 1,
	REVERSE = 2,
	ST_ORDER = 4,
	AOUT = 8
}				t_flags;

typedef struct	s_syminfo{
	char		letter;
	char		*str;
	uint8_t		ext;
	uint64_t	value;
}				t_syminfo;

typedef struct	s_flag_funcs
{
	char 		flag;
	int 		(*func_call)(t_flags *);
} 				t_flag_funcs;

/*
**	flags.c
*/

int		ft_r_flag(t_flags *f);
int		ft_p_flag(t_flags *f);
int		ft_set_flag(char f, t_flags *flags);
int		ft_get_args(int ac, char **av);

/*
**	errors.c
*/

int		ft_puterror(char *str);
int		ft_invalid_flag_error(char flag, int error);

static const t_flag_funcs	g_flag_calls[NB_FLAG] = {
	{'r', &ft_r_flag},
	{'p', &ft_p_flag},
};

static const char			*g_error_table[NB_ERROR] = {
	"Invalid arg : ",
	"Following args may only occur zero or one times! "
};

#endif
