/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opus1io <opus1io@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/11 17:19:41 by opus1io           #+#    #+#             */
/*   Updated: 2019/04/17 18:44:52 by opus1io          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_otool.h>

struct section		*ft_get_section_text_h(struct mach_header *header, \
	struct load_command *lc, bool is_64)
{
	struct section				*section;
	size_t						i;
	size_t						j;

	i = 0;
	while (i++ < header->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64 || lc->cmd == LC_SEGMENT)
		{
			j = 0;
			section = (void *) lc + ((is_64) ? \
				sizeof(struct segment_command_64) : \
				sizeof(struct segment_command));
			while (j < ((is_64) ? ((struct segment_command_64 *)lc)->nsects \
				: ((struct segment_command *)lc)->nsects))
			{
				if (ft_strcmp(section->sectname, SECT_TEXT) == 0)
					return (section);
				section = (void *)section + ((is_64) ? \
					sizeof(struct section_64) : sizeof(struct section));
				j++;
			}
		}
		lc = (void *) lc + lc->cmdsize;
	}
	return (NULL);
}


void			ft_parse_binary(void *ptr, char *file, bool is_64, bool arch)
{
	struct section			*sect;
	uint64_t				i;
	uint64_t				size;
	uint64_t				addr;
	uint32_t				offset;

	if (!(sect = ft_get_section_text_h(ptr, ptr + ((is_64) ? \
		sizeof(struct mach_header_64) : sizeof(struct mach_header)), is_64)))
		return ;
	ft_printf("%s:\nContents of (__TEXT,__text) section", file);
	if (arch)
		free(file);
	i = 0;
	size = (is_64) ? ((struct section_64 *)sect)->size : (uint64_t)(sect->size);
	addr = (is_64) ? ((struct section_64 *)sect)->addr : (uint64_t)(sect->addr);
	offset = (is_64) ? ((struct section_64 *)sect)->offset : sect->offset;
	while (i < size)
	{
		if (i % 16 == 0)
			ft_printf(((is_64) ? "\n%016llx\t%02hhx " : "\n%08lx\t%02hhx "), \
				addr + i, *((uint8_t *)(ptr + offset + i)));
		else
			ft_printf("%02hhx ", *((char *)(ptr + offset + i)));
		i++;
	}
	ft_putchar('\n');
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


void			ft_parse_archive(void *ptr, size_t fsize, char *file)
{
	struct ar_hdr	*files;
	char			*name;
	int				len;
	int				nsize;

	files = ptr + SARMAG;
	while ((void *)files < ptr + fsize)
	{
		name = files->ar_name;
		if (ft_strncmp(files->ar_name, AR_EFMT1, sizeof(AR_EFMT1) - 1) == 0)
		{
			len = ft_atoi(name + sizeof(AR_EFMT1) - 1);
			nsize = ft_atoi(files->ar_size);
			if (ft_strncmp((char *)(files + 1), "__.SYMDEF", 9))
				ft_magic_run((char *)(files + 1) + len, nsize - len,  ft_archive_str((char *)(files + 1), file), true);
		}
		files = (void *)files + sizeof(struct ar_hdr) + nsize;
	}
}

void	ft_magic_run(void *ptr, size_t fsize, char *file, bool arch)
{
	unsigned int	magic;

	magic = *(unsigned int *)ptr;
	if (magic == MH_MAGIC_64)
		ft_parse_binary(ptr, file, true, arch);
	else if (magic == MH_MAGIC)
		ft_parse_binary(ptr, file, false, arch);
	else if (ft_strncmp(ptr, ARMAG, SARMAG) == 0)
	{
		ft_printf("Archive : %s\n", file);
		ft_parse_archive(ptr, fsize, file);
	}
	else
		ft_printf("%s: is not an object file\n", file);
}

void	ft_otool(char *file)
{
	int			fd;
	char		*ptr;
	struct stat	buff;

	if ((fd = open(file, O_RDONLY)) < 0 || (fstat(fd, &buff) < 0))
	{
		ft_printf("%s: Cannot open this file\n", file);
		return ;
	}
	if ((ptr = mmap(0, buff.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) \
		== MAP_FAILED)
		return ;
	ft_magic_run(ptr, buff.st_size, file, false);
	munmap(ptr, buff.st_size);
}

int		main(int ac, char **av)
{
	while (--ac > 0)
		ft_otool(*(++av));
	return (0);
}
