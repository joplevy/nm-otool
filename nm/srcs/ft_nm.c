/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opus1io <opus1io@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 18:10:03 by opus1io           #+#    #+#             */
/*   Updated: 2019/04/17 16:26:33 by opus1io          ###   ########.fr       */
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

char			*ft_archive_str(char *file, char *arch)
{
	char	*ret;
	int		archlen;
	int		filelen;

	filelen = ft_strlen(file);
	archlen = ft_strlen(arch);
	if (!(ret = ft_memalloc(filelen + archlen + 3)))
		return (NULL);
	ft_memcpy(ret, arch, archlen);
	ft_memcpy(ret + archlen + 1, file, filelen);
	ret[archlen] = '(';
	ret[archlen + 1 + filelen] = ')';
	return (ret);
}

void			ft_parse_archive(void *ptr, size_t fsize, t_flags *flags, char *file)
{
	struct ar_hdr	*files;
	char			*name;
	int				len;
	int				nsize;

	files = ptr + SARMAG;
	*flags |= ARCH;
	while ((void *)files < ptr + fsize)
	{
		name = files->ar_name;
		if (ft_strncmp(files->ar_name, AR_EFMT1, sizeof(AR_EFMT1) - 1) == 0)
		{
			len = ft_atoi(name + sizeof(AR_EFMT1) - 1);
			nsize = ft_atoi(files->ar_size);
			if (ft_strncmp((char *)(files + 1), "__.SYMDEF", 9))
				ft_magic_run((char *)(files + 1) + len, nsize - len, flags, ft_archive_str((char *)(files + 1), file));
		}
		files = (void *)files + sizeof(struct ar_hdr) + nsize;
	}
	*flags &= ~ARCH;
}

void		ft_magic_run(void *ptr, size_t fsize, t_flags *flags, char *file)
{
	unsigned int	magic;
	t_list			*list;

	list = NULL;
	magic = *(unsigned int *)ptr;
	if (magic == MH_MAGIC_64)
		list = ft_parse_binary(ptr, true);
	else if (magic == MH_MAGIC)
		list = ft_parse_binary(ptr, false);
	else if (magic == MH_CIGAM || magic == MH_CIGAM_64)
		ft_putendl("reverse");
	else if (magic == FAT_MAGIC || magic == FAT_CIGAM)
		ft_putendl("FAT 32");
	else if (magic == FAT_MAGIC_64 || magic == FAT_CIGAM_64)
		ft_putendl("FAT 64");
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
