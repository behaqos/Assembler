/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnequ.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgian <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/23 16:38:09 by bgian             #+#    #+#             */
/*   Updated: 2019/09/23 16:41:14 by bgian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_strnequ(char const *s1, char const *s2, size_t n)
{
    size_t	i;

    i = 0;
    if (s1 == NULL || s2 == NULL)
        return (0);
    if (n == 0)
        return (1);
    if (s1[i] == s2[i] && n > 0)
    {
        while (s1[i] == s2[i] && s1[i] != '\0'
               && s2[i] != '\0' && n > 0)
        {
            n--;
            if (n == 0)
                break ;
            i++;
        }
        if (s1[i] != s2[i])
            return (0);
        else
            return (1);
    }
    return (0);
}
