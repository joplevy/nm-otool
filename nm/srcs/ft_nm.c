/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opus1io <opus1io@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/21 16:14:23 by opus1io           #+#    #+#             */
/*   Updated: 2019/01/17 16:55:06 by opus1io          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nm.h>
#include <stdio.h>

t_list	*ft_get_syminfos(struct nlist_64 symcmd, char *str, char *sect)
{
	t_list		*ret;
	t_syminfo	infos;
	infos.letter = 0;
	infos.value = symcmd.n_value;
	infos.str = str;
	infos.ext = symcmd.n_type & N_EXT;
	if (symcmd.n_type & N_SECT)
	{
		if (symcmd.n_sect > 0 && symcmd.n_sect <= ft_strlen(sect))
			infos.letter = sect[symcmd.n_sect - 1];
		else
			infos.letter = 'S';
	}
	if (!infos.letter)
		infos.letter = 'U';
	ret = ft_lstnew(&infos, sizeof(infos));
	return (ret);
}

void	ft_get_symbols(struct symtab_command *sym, void *ptr, char *sect, size_t vmsize)
{
	size_t			i;
	char			*strtab;
	struct nlist_64	*symcmd;
	t_list			*res;
	t_syminfo		*infos;

	symcmd = (void *) ptr + sym->symoff;
	strtab = (void *) ptr + sym->stroff;
	i = 0;
	while (i < sym->nsyms)
	{
		res = ft_get_syminfos(symcmd[i], strtab + symcmd[i].n_un.n_strx, sect);
		infos = res->content;
		if (infos->value && infos->value < vmsize)
			ft_printf("%016llx %c %s\n", infos->value, (infos->ext) ? infos->letter : infos->letter + 32, infos->str);
		else if (!(infos->value))
			ft_printf("                 %c %s\n", (infos->ext) ? infos->letter : infos->letter + 32, infos->str);
		i++;
	}
}

size_t	ft_get_nb_sects(struct mach_header_64 *header, struct load_command *lc)
{
	size_t	i;
	size_t	ret;

	i = 0;
	ret = 0;
	while (i++ < header->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
			ret += ((struct segment_command_64 *)lc)->nsects;
		lc = (void *) lc + lc->cmdsize;
	}
	return (ret);
}

size_t	ft_get_sects_size(struct mach_header_64 *header, struct load_command *lc)
{
	size_t	i;
	size_t	ret;

	i = 0;
	ret = 0;
	while (i++ < header->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
			ret += ((struct segment_command_64 *)lc)->vmsize;
		lc = (void *) lc + lc->cmdsize;
	}
	return (ret);
}

char	get_section_letter(char *sectname)
{
	if (ft_strcmp(sectname, SECT_TEXT) == 0)
		return ('T');
	if (ft_strcmp(sectname, SECT_DATA) == 0)
		return ('D');
	if (ft_strcmp(sectname, SECT_BSS) == 0)
		return ('B');
	if (ft_strcmp(sectname, SECT_COMMON) == 0)
		return ('C');
	return ('S');
}

char	*ft_get_sections(struct mach_header_64 *header, struct load_command *lc)
{
	char						*ret;
	struct section_64			*section;
	size_t						i;
	size_t						j;
	size_t						k;

	if ((ret = ft_memalloc(ft_get_nb_sects(header, lc) + 1)) == NULL)
		return (NULL);
	i = 0;
	k = 0;
	while (i++ < header->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
			j = 0;
			section = (void *) lc + sizeof(struct segment_command_64);
			while (j < ((struct segment_command_64 *)lc)->nsects)
			{
				ret[k++] = get_section_letter(section[j].sectname);
				j++;
			}
		}
		lc = (void *) lc + lc->cmdsize;
	}
	return (ret);
}

void	ft_handle_64(void *ptr, char *name)
{
	struct mach_header_64	*header;
	struct load_command		*lc;
	struct symtab_command	*sym;
	char					*sect;
	size_t					i;
	size_t					memsize;

	header = (struct mach_header_64 *)ptr;
	lc = (void *) ptr + sizeof(*header);
	i = 0;
	memsize = ft_get_sects_size(ptr, lc);
	sect = ft_get_sections(ptr, lc);
	while (i++ < header->ncmds && name)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *) lc;
			ft_get_symbols(sym, ptr, sect, memsize);
			return ;
		}
		lc = (void *) lc + lc->cmdsize;
	}
}

void	ft_parse_binary(void *ptr, char *name)
{
	unsigned int	magic;

	magic = *(unsigned int *)ptr;
	if (magic != MH_MAGIC_64)
	{
		ft_printf("./ft_nm: %s: The file was not recognized as a valid \
object file\n\n",
		name);
		return;
	}
	else
		ft_handle_64(ptr, name);
}

void	ft_process_file(char *file)
{
	int			fd;
	char		*ptr;
	struct stat	buff;

	if ((fd = open(file, O_RDONLY)) < 0 || (fstat(fd, &buff) < 0))
	{
		ft_printf("./ft_nm: %s: Cannot open this file\n\n", file);
		return;
	}
	if ((ptr = mmap(0, buff.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return;
	ft_parse_binary(ptr, file);
	munmap(ptr, buff.st_size);
}

void	ft_handle_files(int ac, char **av)
{
	while (--ac >= 0)
	{
		if (ft_strlen(*av) > 0)
			ft_process_file(*av);
		av++;
	}
}

void	ft_nm(int ac, char **av, t_flags flags)
{
	if (flags & AOUT)
		ft_process_file("a.out");
	else if (ac > 0 && (av))
		ft_handle_files(ac, av);
}

int		main(int ac, char **av)
{
	t_flags	flags;

	if ((int)(flags = (t_flags)ft_get_args(--ac, ++av)) < 0)
		return (-1);
	ft_nm(ac, av, flags);
	return (0);
}
