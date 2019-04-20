/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_swap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opus1io <opus1io@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/19 18:39:56 by opus1io           #+#    #+#             */
/*   Updated: 2019/04/19 18:40:31 by opus1io          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_nm.h>

uint32_t	swap_int32(uint32_t data)
{
	return (((data & 0xff000000) >> 24) |
			((data & 0x00ff0000) >> 8) |
			((data & 0x0000ff00) << 8) |
			((data & 0x000000ff) << 24));
}

uint64_t	swap_int64(uint64_t data)
{
	return ((((data & 0xff00000000000000ULL) >> 56) |
			((data & 0x00ff000000000000ULL) >> 40) |
			((data & 0x0000ff0000000000ULL) >> 24) |
			((data & 0x000000ff00000000ULL) >>  8) |
			((data & 0x00000000ff000000ULL) <<  8) |
			((data & 0x0000000000ff0000ULL) << 24) |
			((data & 0x000000000000ff00ULL) << 40) |
			((data & 0x00000000000000ffULL) << 56)));
}

uint32_t	get_int32_value(uint32_t data, bool is_swap)
{
	return (is_swap ? swap_int32(data) : data);
}

uint64_t	get_int64_value(uint64_t data, bool is_swap)
{
	return (is_swap ? swap_int64(data) : data);
}
