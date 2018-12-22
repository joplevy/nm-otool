/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jplevy <jplevy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/07 11:44:20 by jplevy            #+#    #+#             */
/*   Updated: 2016/01/09 03:54:41 by jplevy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcat(char *s1, const char *s2)
{
	char	*tmp1;
	char	*tmp2;

	tmp1 = s1;
	tmp2 = (char *)s2;
	while (*tmp1 != '\0')
		tmp1++;
	while (*tmp2 != '\0')
	{
		*tmp1 = *tmp2;
		tmp1++;
		tmp2++;
	}
	*tmp1 = '\0';
	return (s1);
}
