/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opavliuk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/23 19:15:47 by opavliuk          #+#    #+#             */
/*   Updated: 2018/03/28 20:22:50 by opavliuk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strsub(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*res;

	if (!s)
		return (0);
	res = (char *)ft_strnew(sizeof(*res) * len);
	if (!res)
		return (0);
	i = -1;
	while (++i < len)
		res[i] = s[start + i];
	return (res);
}
