/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jplevy <jplevy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/21 16:14:23 by opus1io           #+#    #+#             */
/*   Updated: 2019/03/02 14:27:27 by jplevy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <ft_nm.h>

int		main(int ac, char **av)
{
	t_flags	flags;

	if ((int)(flags = (t_flags)ft_get_args(--ac, ++av)) < 0)
		return (-1);
	ft_nm(ac, av, flags);
	return (0);
}
