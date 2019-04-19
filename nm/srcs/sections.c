/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sections.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opus1io <opus1io@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 18:14:50 by opus1io           #+#    #+#             */
/*   Updated: 2019/04/18 18:29:29 by opus1io          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nm.h>

static size_t	ft_get_nb_sects(struct mach_header *header, \
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
		if (lc->cmd == LC_SEGMENT)
			ret += ((struct segment_command *)lc)->nsects;
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

char			*ft_get_sections(struct mach_header *header, \
	struct load_command *lc, bool is_64)
{
	char						*ret;
	struct section				*section;
	size_t						i;
	size_t						j;
	size_t						k;

	if ((ret = ft_memalloc(ft_get_nb_sects(header, lc) + 1)) == NULL)
		return (NULL);
	i = 0;
	k = 0;
	while (i++ < header->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64 || lc->cmd == LC_SEGMENT)
		{
			j = 0;
			section = (void *) lc + ((is_64) ? sizeof(struct segment_command_64) \
				: sizeof(struct segment_command));
			while (j < ((is_64) ? ((struct segment_command_64 *)lc)->nsects \
				: ((struct segment_command *)lc)->nsects))
			{
				ret[k++] = get_section_letter(section->sectname);
				section = (void *)section + ((is_64) ? sizeof(struct section_64) : sizeof(struct section));
				j++;
			}
		}
		lc = (void *) lc + lc->cmdsize;
	}
	return (ret);
}
