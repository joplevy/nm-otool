/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opus1io <opus1io@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/11 17:19:41 by opus1io           #+#    #+#             */
/*   Updated: 2019/02/13 14:15:25 by opus1io          ###   ########.fr       */
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


void			ft_parse_binary(void *ptr, char *file, bool is_64)
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

void	ft_magic_run(void *ptr, char *file)
{
	unsigned int	magic;

	magic = *(unsigned int *)ptr;
	if (magic == MH_MAGIC_64)
		ft_parse_binary(ptr, file, true);
	else if (magic == MH_MAGIC)
		ft_parse_binary(ptr, file, false);
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
	ft_magic_run(ptr, file);
	munmap(ptr, buff.st_size);
}

int		main(int ac, char **av)
{
	while (--ac > 0)
		ft_otool(*(++av));
	return (0);
}
