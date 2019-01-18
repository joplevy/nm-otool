/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sections.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opus1io <opus1io@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 18:14:50 by opus1io           #+#    #+#             */
/*   Updated: 2019/01/18 13:48:58 by opus1io          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nm.h>

static size_t	ft_get_nb_sects(struct mach_header_64 *header, \
	struct load_command *lc)
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

static char		get_section_letter(char *sectname)
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

char			*ft_get_sections(struct mach_header_64 *header, \
	struct load_command *lc)
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
