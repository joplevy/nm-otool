/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opus1io <opus1io@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 18:28:54 by jplevy            #+#    #+#             */
/*   Updated: 2019/01/17 18:35:54 by opus1io          ###   ########.fr       */
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
	REVERSE = 1,
	ST_ORDER = 2,
	AOUT = 4
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
**	main.c
*/

void	ft_print_symbols(t_list *list);

/*
**	symbols.c
**
** static t_list	*ft_get_syminfos(struct nlist_64 symcmd, char *str, char *sect);
** static t_list	*ft_get_symbols(struct symtab_command *sym, void *ptr, char *sect);
**
*/

t_list	*ft_handle_64(void *ptr);

/*
**	sections.c
**
** static size_t	ft_get_nb_sects(struct mach_header_64 *header, struct load_command *lc);
** static char		get_section_letter(char *sectname);
**
*/

char	*ft_get_sections(struct mach_header_64 *header, struct load_command *lc);

/*
**	ft_nm.c
**
** static t_list	*ft_parse_binary(void *ptr);
** static void		ft_process_file(char *file);
** static void		ft_handle_files(int ac, char **av);
**
*/

void	ft_nm(int ac, char **av, t_flags flags);

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
