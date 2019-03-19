/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jplevy <jplevy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 18:10:03 by opus1io           #+#    #+#             */
/*   Updated: 2019/03/18 18:24:14 by jplevy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nm.h>
#include <unistd.h>

static void		ft_print_line(t_syminfo *infos, bool is_64)
{
	if (ft_strlen(infos->str) == 0)
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

	if (!(flags & ST_ORDER))
	{
		ft_sort_alpha(&list);
		if ((flags & REVERSE))
			ft_reverse_list(&list);
	}
	if (flags & MANY)
		ft_printf("\n%s:\n", file);
	tmp = list;
	while (tmp)
	{
		ft_print_line(tmp->content, is_64);
		tmp = tmp->next;
	}
}

void			ft_parse_archive(void *ptr, size_t fsize)
{
	struct ar_hdr	*files;
	char			*name;
	int				len;

	files = ptr + SARMAG;
	while ((void *)files < ptr + fsize)
	{
		write(1, files->ar_name, 16);
		name = files->ar_name;
		if (ft_strncmp(files->ar_name, AR_EFMT1, sizeof(AR_EFMT1) - 1))
			ft_putendl("qqchose here");
		write(1, "|\n", 2);
		// write(1, files + 1, 16);
		ft_putendl((char *)(files + 1));
		// write(1, files->ar_size, 10);
		len = ft_atoi(name + 3);
		// ft_putendl(files->ar_size);
		files = (void *)files + sizeof(struct ar_hdr) + ft_atoi(files->ar_size);
		ft_printf("%d : %02X\n", len, *(unsigned int *)((char *)(files + 1) + len));
	}
	// return;
}

static t_list	*ft_magic_run(void *ptr, size_t fsize)
{
	unsigned int	magic;

	magic = *(unsigned int *)ptr;
	if (magic == MH_MAGIC_64)
		return (ft_parse_binary(ptr, true));
	if (magic == MH_MAGIC)
		return (ft_parse_binary(ptr, false));
	if (magic == MH_CIGAM || magic == MH_CIGAM_64)
		ft_putendl("reverse");
	if (magic == FAT_MAGIC || magic == FAT_CIGAM)
		ft_putendl("FAT 32");
	if (magic == FAT_MAGIC_64 || magic == FAT_CIGAM_64)
		ft_putendl("FAT 64");
	if (ft_strncmp(ptr, ARMAG, SARMAG) == 0)
		ft_parse_archive(ptr, fsize);
	// ft_printf("%x\n", magic);
	return (NULL);
}

static void		ft_process_file(char *file, t_flags flags)
{
	int			fd;
	char		*ptr;
	struct stat	buff;
	t_list		*list;

	if ((fd = open(file, O_RDONLY)) < 0 || (fstat(fd, &buff) < 0))
	{
		ft_printf("./ft_nm: %s: Cannot open this file\n\n", file);
		return ;
	}
	if ((ptr = mmap(0, buff.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) \
		== MAP_FAILED)
		return ;
	if ((list = ft_magic_run(ptr, buff.st_size)) == NULL)
		ft_printf("./ft_nm: %s: The file was not recognized \
as a valid object file\n\n", file);
	ft_print_symbols(list, flags, file, \
		(*(unsigned int *)ptr == MH_MAGIC_64) ? true : false);
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
