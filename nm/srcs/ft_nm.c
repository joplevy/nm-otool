/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opus1io <opus1io@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/21 16:14:23 by opus1io           #+#    #+#             */
/*   Updated: 2018/12/22 12:47:31 by opus1io          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nm.h>

int		ft_puterror(char *str)
{
	ft_putstr_fd(str, 2);
	return (-1);
}

int				ft_r_flag(t_flags *f)
{
	*f ^= REVERSE;
	if (*f & REVERSE)
		return (0);
	else
		return (-1);
}

int				ft_p_flag(t_flags *f)
{
	*f ^= ST_ORDER;
	if (*f & ST_ORDER)
		return (0);
	else
		return (-1);
}

int		ft_set_flag(char f, t_flags *flags)
{
	int		i;

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

int		ft_invalid_flag_error(char flag, int error)
{
	error =  -(error) - 1;
	ft_putstr_fd(g_error_table[error], 2);
	ft_putchar_fd(flag, 2);
	ft_putchar_fd('\n', 2);
	return (-1);
}

int		ft_get_args(int ac, char **av)
{
	int		lac;
	char	**lav;
	t_flags	flags;
	int		error;

	lac = ac;
	lav = av;
	flags = 1;
	while (--lac >= 0)
	{
		if (**lav == '-')
		{
			while (*(++(*lav)))
			{
				if ((error = ft_set_flag(**lav, &flags)) < 0)
					return (ft_invalid_flag_error(**lav, error));
			}
		}
		ft_putendl(*lav);
		lav++;
	}
	return (0);
}

int		main(int ac, char **av)
{
	if (ac < 2)
		return (ft_puterror(USAGE));
	if ((ft_get_args(ac - 1, av + 1)) < 0)
		return (-1);
	return (0);
}
