/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linaina <linaina@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 17:35:20 by yfawzi            #+#    #+#             */
/*   Updated: 2023/08/27 23:56:25 by linaina          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	cha;

	i = 0;
	cha = (char )c;
	while (s[i])
	{
		if (s[i] == cha)
			return ((char *)&s[i]);
		i++;
	}
	if (s[i] == cha)
		return ((char *)&s[i]);
	return (NULL);
}
