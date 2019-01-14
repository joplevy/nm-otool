/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opus1io <opus1io@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/22 15:23:28 by opus1io           #+#    #+#             */
/*   Updated: 2018/12/22 15:24:16 by opus1io          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nm.h>

int		ft_puterror(char *str)
{
	ft_putstr_fd(str, 2);
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
