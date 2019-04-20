/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ach_fat.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opus1io <opus1io@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/19 18:43:47 by opus1io           #+#    #+#             */
/*   Updated: 2019/04/19 18:46:21 by opus1io          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nm.h>

static char			*ft_archive_str(char *file, char *arch)
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

void			ft_parse_fat(void *ptr, t_boolinfo info, t_flags *flags, char *file)
{
	struct fat_header	*header;
	void				*arch;
	void				*filestart;
	uint64_t			filesize;
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
			filesize = (info.is_64) ? get_int64_value(((struct fat_arch_64 *)arch)->size, info.is_swap) : \
				(uint64_t) get_int32_value(((struct fat_arch *)arch)->size, info.is_swap);
			ft_magic_run(filestart, filesize, flags, file);
			return ;
		}
		arch += (info.is_64) ? sizeof(struct fat_arch_64) : sizeof(struct fat_arch);
		i++;
	}
	ft_printf("./ft_nm: %s: The file was not recognized \
as a valid fat file\n\n", file);
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
