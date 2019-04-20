/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opus1io <opus1io@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 18:10:03 by opus1io           #+#    #+#             */
/*   Updated: 2019/04/19 18:44:16 by opus1io          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nm.h>
#include <unistd.h>

static void		ft_print_line(t_syminfo *infos, bool is_64)
{
	if (ft_strlen(infos->str) == 0 || infos->letter == 0)
		return ;
	if (is_64)
	{
		if (infos->letter != 'U')
			ft_printf("%016llx %c %s\n", infos->value,
				(infos->ext) ? infos->letter : infos->letter + 32, infos->str);
		else
			ft_printf("                 %c %s\n", (infos->ext) ? infos->letter
				: infos->letter + 32, infos->str);
		return ;
	}
	if (infos->letter != 'U')
		ft_printf("%08lx %c %s\n", infos->value,
			(infos->ext) ? infos->letter : infos->letter + 32, infos->str);
	else
		ft_printf("         %c %s\n", (infos->ext) ? infos->letter
			: infos->letter + 32, infos->str);
}

static void		ft_print_symbols(t_list *list, t_flags flags, char *file, \
	bool is_64)
{
	t_list		*tmp;

	if (!list)
		return ;
	if (!(flags & ST_ORDER))
	{
		ft_sort_alpha(&list);
		if ((flags & REVERSE))
			ft_reverse_list(&list);
	}
	if (flags & MANY)
	{
		ft_printf("\n%s:\n", file);
		if (flags & ARCH)
			free(file);
	}
	tmp = list;
	while (tmp)
	{
		ft_print_line(tmp->content, is_64);
		tmp = tmp->next;
	}
}

void		ft_magic_run(void *ptr, size_t fsize, t_flags *flags, char *file)
{
	unsigned int	magic;
	t_list			*list;
	t_boolinfo		info;

	list = NULL;
	magic = *(unsigned int *)ptr;
	info.is_64 = (magic == MH_MAGIC_64 || magic == MH_CIGAM_64 || magic == FAT_MAGIC_64 || magic == FAT_CIGAM_64);
	info.is_swap = (magic == MH_CIGAM || magic == MH_CIGAM_64 || magic == FAT_CIGAM || magic == FAT_CIGAM_64);
	if (magic == MH_MAGIC_64 || magic == MH_MAGIC)
		list = ft_parse_binary(ptr, info.is_64);
	else if (magic == FAT_MAGIC_64 || magic == FAT_CIGAM_64 || magic == FAT_MAGIC || magic == FAT_CIGAM)
		ft_parse_fat(ptr, info, flags, file);
	else if (ft_strncmp(ptr, ARMAG, SARMAG) == 0)
	{
		*flags |= MANY;
		ft_parse_archive(ptr, fsize, flags, file);
	}
	else
		ft_printf("./ft_nm: %s: The file was not recognized \
as a valid object file\n\n", file);
	ft_print_symbols(list, *flags, file, \
		(*(unsigned int *)ptr == MH_MAGIC_64) ? true : false);
}

static void		ft_process_file(char *file, t_flags flags)
{
	int			fd;
	char		*ptr;
	struct stat	buff;

	if ((fd = open(file, O_RDONLY)) < 0 || (fstat(fd, &buff) < 0))
	{
		ft_printf("./ft_nm: %s: Cannot open this file\n\n", file);
		return ;
	}
	if ((ptr = mmap(0, buff.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) \
		== MAP_FAILED)
		return ;
	ft_magic_run(ptr, buff.st_size, &flags, file);
	munmap(ptr, buff.st_size);
}

void			ft_nm(int ac, char **av, t_flags flags)
{
	if (flags & AOUT)
		ft_process_file("a.out", flags);
	else if (ac > 0 && (av))
	{
		while (--ac >= 0)
		{
			if (ft_strlen(*av) > 0)
				ft_process_file(*av, flags);
			av++;
		}
	}
}
