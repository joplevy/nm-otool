/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opus1io <opus1io@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/21 16:14:23 by opus1io           #+#    #+#             */
/*   Updated: 2019/01/17 18:37:15 by opus1io          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nm.h>

void	ft_print_symbols(t_list *list)
{
	t_list		*tmp;
	t_syminfo	*infos;

	tmp = list;
	while (tmp)
	{
		infos = tmp->content;
		if (infos->value)
			ft_printf("%016llx %c %s\n", infos->value, (infos->ext) ? infos->letter : infos->letter + 32, infos->str);
		else if (!(infos->value))
			ft_printf("                 %c %s\n", (infos->ext) ? infos->letter : infos->letter + 32, infos->str);
		tmp = tmp->next;
	}
}

int		main(int ac, char **av)
{
	t_flags	flags;

	if ((int)(flags = (t_flags)ft_get_args(--ac, ++av)) < 0)
		return (-1);
	ft_nm(ac, av, flags);
	return (0);
}
