/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opus1io <opus1io@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/11 17:20:59 by opus1io           #+#    #+#             */
/*   Updated: 2019/04/17 18:41:05 by opus1io          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_OTOOL_H
# define FT_OTOOL_H

# include <libft.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/mman.h>
 #include <stdlib.h>
# include <stdbool.h>

# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <ar.h>

void	ft_magic_run(void *ptr, size_t fsize, char *file, bool arch);

#endif
