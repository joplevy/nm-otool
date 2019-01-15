/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opus1io <opus1io@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/21 16:14:23 by opus1io           #+#    #+#             */
/*   Updated: 2019/01/14 18:05:07 by opus1io          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nm.h>

void	ft_get_symbols(struct symtab_command *sym, void *ptr)
{
	size_t			i;
	char			*strtab;
	struct nlist_64	*symcmd;

	symcmd = (void *)ptr + sym->symoff;
	strtab = (void *) ptr + sym->stroff;
	i = 0;
	while (i < sym->nsyms)
	{
		ft_printf("%s\n", strtab + symcmd[i].n_un.n_strx);
		i++;
	}
}


void	ft_handle_64(void *ptr, char *name)
{
	struct mach_header_64	*header;
	struct load_command		*lc;
	struct symtab_command	*sym;
	size_t					i;

	header = (struct mach_header_64 *)ptr;
	lc = (void *) ptr + sizeof(*header);
	i = 0;
	while (i++ < header->ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *) lc;
			ft_printf("%s:\n", name);
			ft_get_symbols(sym, ptr);
			return ;
		}
		lc = (void *) lc + lc->cmdsize;
	}
}

void	ft_parse_binary(void *data, char *name)
{
	unsigned int	magic;

	magic = *(unsigned int *)data;
	if (magic != MH_MAGIC_64)
	{
		ft_printf("./ft_nm: %s: The file was not recognized as a valid \
object file\n\n",
		name);
		return;
	}
	else
		ft_handle_64(data, name);
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
