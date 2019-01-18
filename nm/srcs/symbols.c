/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbols.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opus1io <opus1io@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 18:16:56 by opus1io           #+#    #+#             */
/*   Updated: 2019/01/17 18:25:26 by opus1io          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nm.h>

static t_list	*ft_get_syminfos(struct nlist_64 symcmd, char *str, char *sect)
{
	t_list		*ret;
	t_syminfo	infos;
	infos.letter = 0;
	infos.value = symcmd.n_value;
	infos.str = str;
	infos.ext = symcmd.n_type & N_EXT;
	if (symcmd.n_type & N_SECT)
	{
		if (symcmd.n_sect > 0 && symcmd.n_sect <= ft_strlen(sect) + 1)
			infos.letter = sect[symcmd.n_sect - 1];
		else
			return (NULL);
	}
	if (!infos.letter)
		infos.letter = 'U';
	ret = ft_lstnew(&infos, sizeof(infos));
	return (ret);
}

static t_list	*ft_get_symbols(struct symtab_command *sym, void *ptr, char *sect)
{
	size_t			i;
	struct nlist_64	*symcmd;
	t_list			*node;
	t_list			*list;

	symcmd = (void *) ptr + sym->symoff;
	list = NULL;
	i = 0;
	while (i < sym->nsyms)
	{
		if ((node = ft_get_syminfos(symcmd[i], ptr + sym->stroff + symcmd[i].n_un.n_strx, sect)))
			ft_lstadd_back(&list, node);
		i++;
	}
	return (list);
}

t_list	*ft_handle_64(void *ptr)
{
	struct mach_header_64	*header;
	struct load_command		*lc;
	struct symtab_command	*sym;
	char					*sect;
	size_t					i;

	header = (struct mach_header_64 *)ptr;
	lc = (void *) ptr + sizeof(*header);
	i = 0;
	sect = ft_get_sections(ptr, lc);
	while (i++ < header->ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *) lc;
			return (ft_get_symbols(sym, ptr, sect));
		}
		lc = (void *) lc + lc->cmdsize;
	}
	return (NULL);
}
