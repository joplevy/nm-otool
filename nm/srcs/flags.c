/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opus1io <opus1io@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/22 15:25:23 by opus1io           #+#    #+#             */
/*   Updated: 2019/01/18 11:32:39 by opus1io          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nm.h>

int			ft_r_flag(t_flags *f)
{
	*f ^= REVERSE;
	if (*f & REVERSE)
		return (0);
	else
		return (-1);
}

int			ft_p_flag(t_flags *f)
{
	*f ^= ST_ORDER;
	if (*f & ST_ORDER)
		return (0);
	else
		return (-1);
}

static int	ft_set_flag(char f, t_flags *flags)
{
	int i;

	i = -1;
	while (++i < NB_FLAG)
	{
		if (g_flag_calls[i].flag == f)
		{
			if ((g_flag_calls[i].func_call(flags)) < 0)
				return (-2);
			else
				return (0);
		}
	}
	return (-1);
}

int			ft_get_args(int ac, char **av)
{
	int lac;
	char **lav;
	t_flags flags;
	int error;
	int nb_f;

	lac = ac;
	lav = av;
	flags = AOUT;
	nb_f = 0;
	while (--lac >= 0)
	{
		if (**lav == '-')
		{
			while (*(++(*lav)))
			{
				if ((error = ft_set_flag(**lav, &flags)) < 0)
					return (ft_invalid_flag_error(**lav, error));
			}
			nb_f++;
		}
		else
			flags &= (~AOUT);
		lav++;
	}
	return ((int)flags);
}
