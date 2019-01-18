/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opus1io <opus1io@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 18:10:03 by opus1io           #+#    #+#             */
/*   Updated: 2019/01/17 18:34:37 by opus1io          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nm.h>

static t_list	*ft_parse_binary(void *ptr)
{
	unsigned int	magic;

	magic = *(unsigned int *)ptr;
	if (magic != MH_MAGIC_64)
		return (NULL);
	else
		return (ft_handle_64(ptr));
}

static void	ft_process_file(char *file)
{
	int			fd;
	char		*ptr;
	struct stat	buff;
	t_list		*list;

	if ((fd = open(file, O_RDONLY)) < 0 || (fstat(fd, &buff) < 0))
	{
		ft_printf("./ft_nm: %s: Cannot open this file\n\n", file);
		return;
	}
	if ((ptr = mmap(0, buff.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return;
	if ((list = ft_parse_binary(ptr)) == NULL)
		ft_printf("./ft_nm: %s: The file was not recognized as a valid object file\n\n", file);
	ft_print_symbols(list);
	munmap(ptr, buff.st_size);
}

static void	ft_handle_files(int ac, char **av)
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

