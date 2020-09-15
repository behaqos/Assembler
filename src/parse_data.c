/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpenney <dpenney@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 06:21:44 by dpenney           #+#    #+#             */
/*   Updated: 2020/09/15 06:44:54 by dpenney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

unsigned            *parse_t_reg(char *arg, int *detector)
{
	unsigned        *value_num;

	if (!(value_num = (unsigned *)ft_memalloc(sizeof(unsigned))))
		return (NULL);
	*value_num = ft_atoi(arg + 1);
	*detector = NUM_VAL;
	return ((void *)value_num);
}

/*
 * Plus 2 than to miss symbol ':' and space or tabulation.
 */

char                *parse_data_text(char *arg, char *str, int *detector, int i)
{
	if (!(str = ft_strsub(arg + i, 0, ft_strlen(arg) - 1)))
		error_printf(NULL, ERROR_ALLOCATE, NULL);
	*detector = STRING_VAL;
	return ((void *) str);

}

char                *parse_data_num(char *arg, int *detector, int i)
{
	unsigned        *value_num;

	if (!(value_num = (unsigned *)ft_memalloc(sizeof(unsigned))))
		return (NULL);
	*value_num = (unsigned)ft_atoi(arg + i);
	*detector = NUM_VAL;
	return ((void *)value_num);
}

/*
 * в этой функции в зависимости от типа аргумента парсим данные.
 * Если это T_REG, то парсим цифру от r переменной.
 * Если аргумент начинается с %, а дальше : - значит это метка, копируем её как строку.
 * Если просто %, то парсим самое значение как цифру.
 */

void                *parse_data(char *arg, int arg_type, int *detector)
{
	int             i;
	char            *str;

	i = 0;
	str = NULL;
	if (arg_type == T_REG)
		return (parse_t_reg(arg, &*detector));
	else
	{
		if (arg[i] == '%')
			i++;
		if (arg[i] == ':')
			return (parse_data_text(arg, &*str, &*detector, ++i));
		else
			return (parse_data_num(arg, &*detector, i));
	}
}
