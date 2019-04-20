/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbols.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opus1io <opus1io@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 18:16:56 by opus1io           #+#    #+#             */
/*   Updated: 2019/04/19 18:13:20 by opus1io          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nm.h>

static t_list	*ft_get_syminfos(struct nlist_64 symcmd, char *str, char *sect, \
	bool is_64)
{
	t_list		*ret;
	t_syminfo	infos;

	infos.letter = 0;
	infos.value = (is_64) ? symcmd.n_value : (uint32_t)(symcmd.n_value);
	infos.str = str;
	infos.ext = symcmd.n_type & N_EXT;
	if ((symcmd.n_type & N_TYPE) == N_SECT)
	{
		if (symcmd.n_sect > 0 && symcmd.n_sect <= ft_strlen(sect) + 1)
			infos.letter = sect[symcmd.n_sect - 1];
		else
			return (NULL);
	}
	if ((symcmd.n_type & N_TYPE) == N_UNDF)
		infos.letter = 'U';
	if ((symcmd.n_type & N_TYPE) == N_ABS)
		infos.letter = 'A';
	ret = ft_lstnew(&infos, sizeof(infos));
	return (ret);
}

static t_list	*ft_get_symbols(struct symtab_command *sym, void *ptr, char *sect, \
	bool is_64)
{
	size_t			i;
	void			*symcmd;
	t_list			*node;
	t_list			*list;

	symcmd = (void *) ptr + sym->symoff;
	list = NULL;
	i = 0;
	while (i < sym->nsyms)
	{
		if ((node = ft_get_syminfos(*(struct nlist_64 *)symcmd, ptr + sym->stroff \
			+ ((struct nlist *)symcmd)->n_un.n_strx, sect, is_64)))
			ft_lstadd_back(&list, node);
		symcmd += ((is_64) ? sizeof(struct nlist_64) : sizeof(struct nlist));
		i++;
	}
	return (list);
}

t_list			*ft_parse_binary(void *ptr, bool is_64)
{
	struct mach_header		*header;
	struct load_command		*lc;
	struct symtab_command	*sym;
	char					*sect;
	size_t					i;

	header = (struct mach_header *)ptr;
	lc = (void *) ptr + ((is_64) ? sizeof(struct mach_header_64)
		: sizeof(struct mach_header));
	i = 0;
	sect = ft_get_sections(ptr, lc, is_64);
	while (i++ < header->ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (void *) lc;
			return (ft_get_symbols(sym, ptr, sect, is_64));
		}
		lc = (void *) lc + lc->cmdsize;
	}
	return (NULL);
}
