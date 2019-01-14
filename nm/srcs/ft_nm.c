/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opus1io <opus1io@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/21 16:14:23 by opus1io           #+#    #+#             */
/*   Updated: 2019/01/14 12:44:47 by opus1io          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nm.h>

void ft_nm(int ac, char **av, t_flags flags)
{
	if (flags & AOUT)
		ft_putendl("read a.out");
	else if (ac > 0 && (av))
		ft_putendl("read all files");
}

int main(int ac, char **av)
{
	t_flags flags;

	// if (ac < 2)
	// 	return (ft_puterror(USAGE));
	if ((flags = (t_flags)ft_get_args(--ac, ++av)) < 0)
		return (-1);
	ft_nm(ac, av, flags);
	return (0);
}
