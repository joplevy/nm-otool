/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opus1io <opus1io@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 18:10:03 by opus1io           #+#    #+#             */
/*   Updated: 2019/04/18 17:59:07 by opus1io          ###   ########.fr       */
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


uint32_t	swap_int32(uint32_t data)
{
	return (((data & 0xff000000) >> 24) |
			((data & 0x00ff0000) >> 8) |
			((data & 0x0000ff00) << 8) |
			((data & 0x000000ff) << 24));
}

uint64_t	swap_int64(uint64_t data)
{
	return ((((data & 0xff00000000000000ULL) >> 56) |
			((data & 0x00ff000000000000ULL) >> 40) |
			((data & 0x0000ff0000000000ULL) >> 24) |
			((data & 0x000000ff00000000ULL) >>  8) |
			((data & 0x00000000ff000000ULL) <<  8) |
			((data & 0x0000000000ff0000ULL) << 24) |
			((data & 0x000000000000ff00ULL) << 40) |
			((data & 0x00000000000000ffULL) << 56)));
}

uint32_t	get_int32_value(uint32_t data, bool is_swap)
{
	return (is_swap ? swap_int32(data) : data);
}

uint64_t	get_int64_value(uint64_t data, bool is_swap)
{
	return (is_swap ? swap_int64(data) : data);
}

void			ft_parse_fat(void *ptr, t_boolinfo info, t_flags *flags, char *file)
{
	struct fat_header	*header;
	void				*arch;
	void				*filestart;
	size_t				i;

	header = ptr;
	i = 0;
	arch = ptr + sizeof(struct fat_header);
	while (i < get_int32_value(header->nfat_arch, info.is_swap))
	{
		if (get_int32_value(((struct fat_arch *)arch)->cputype, info.is_swap) == CPU_TYPE_X86_64)
		{
			filestart = (info.is_64)? ptr + get_int64_value(((struct fat_arch_64 *)arch)->offset, info.is_swap) : \
				ptr + get_int32_value(((struct fat_arch *)arch)->offset, info.is_swap);
			ft_magic_run(filestart, 0, flags, file);
			return ;
		}
		arch += (info.is_64) ? sizeof(struct fat_arch_64) : sizeof(struct fat_arch);
		i++;
	}
	ft_printf("./ft_nm: %s: The file was not recognized \
as a valid object file. fat file doesn't contain x84_64 arch\n\n", file);
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
