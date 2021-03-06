/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opus1io <opus1io@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 18:28:54 by jplevy            #+#    #+#             */
/*   Updated: 2019/04/19 18:46:10 by opus1io          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

# include <libft.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <stdbool.h>
 #include <stdlib.h>

# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <mach/machine.h>
# include <ar.h>

# define NB_FLAG 2
# define NB_ERROR 3

typedef enum	e_flags
{
	NONE = 0,
	REVERSE = 1,
	ST_ORDER = 2,
	AOUT = 4,
	MANY = 8,
	ARCH = 16
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

typedef struct	s_boolinfo{
	bool		is_64;
	bool		is_swap;
}				t_boolinfo;


/*
**	ft_swap.c
*/
uint32_t	swap_int32(uint32_t data);
uint64_t	swap_int64(uint64_t data);
uint32_t	get_int32_value(uint32_t data, bool is_swap);
uint64_t	get_int64_value(uint64_t data, bool is_swap);

/*
**	arch_fat.c
**
** static char			*ft_archive_str(char *file, char *arch);
*/

void	ft_parse_archive(void *ptr, size_t fsize, t_flags *flags, char *file);
void	ft_parse_fat(void *ptr, t_boolinfo info, t_flags *flags, char *file);

/*
**	merge_sort.c
** static t_list	*ft_sorted_merge(t_list* a, t_list* b)
** static void		ft_split_list(t_list* list, t_list** front, t_list** back)
*/

void	ft_sort_alpha(t_list **list);
void	ft_reverse_list(t_list **list);

/*
**	symbols.c
**
** static t_list	*ft_get_syminfos(struct nlist_64 symcmd, char *str, char *sect);
** static t_list	*ft_get_symbols(struct symtab_command *sym, void *ptr, char *sect);
**
*/

t_list	*ft_parse_binary(void *ptr, bool is_64);

/*
**	sections.c
**
** static size_t	ft_get_nb_sects(struct mach_header_64 *header, struct load_command *lc);
** static char		get_section_letter(char *sectname);
**
*/

char	*ft_get_sections(struct mach_header *header, struct load_command *lc, bool is_64);

/*
**	ft_nm.c
**
** static t_list	*ft_parse_binary(void *ptr);
** static void		ft_process_file(char *file, t_flags flags);
** static void		ft_handle_files(int ac, char **av);
**
*/

void	ft_magic_run(void *ptr, size_t fsize, t_flags *flags, char *file);
void	ft_nm(int ac, char **av, t_flags flags);

/*
**	flags.c
**
** static int		ft_set_flag(char f, t_flags *flags);
**
*/

int		ft_r_flag(t_flags *f);
int		ft_p_flag(t_flags *f);
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
