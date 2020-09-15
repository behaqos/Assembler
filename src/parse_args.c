/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpenney <dpenney@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 06:21:40 by dpenney           #+#    #+#             */
/*   Updated: 2020/09/15 06:44:54 by dpenney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

/*
 * This function cleaning strings of arguments of tabs and spaces.
 */
char           		*clean_voids(char *str)
{
	int             x;
	int             y;
    char            *res;

	if (!str|| !*str)
		error_printf(NULL, "ERROR\n", NULL);
	x = 0;
	while (str[x] && (str[x] == ' ' ||
			str[x] == '\n' || str[x] == '\t'))
		x++;
	y = ft_strlen(str) - 1;
	while (x < y && ((str)[y] == ' ' ||
	        (str)[y] == '\n' ||
            (str)[y] == '\t'))
	    y--;
	y = y - x + 1;
	if (!(res = ft_strsub(str, x, y)))
	    error_printf(NULL, ERROR_ALLOCATE, NULL);
	free(str);
	return (res);
}

/*
 * Запускаем аргументы для подбора в цикле.
 * Внутри цикла следующее условие:
 * 1. Найти первое вхождение аргумента в строке и вычислить строку + длина текста аргумента.
 * Так мы передвинемся к необходимой позиции после первого аргумента.
 * Если есть второй аргумент, то находим позицию второго аргумента.
 * Далее ищем запятую между первым и вторым аргументами. Если запятой больше, чем одна, то ошибка.
 */

void                check_commas(t_asm *bler, char **args)
{
	int             i;
	int             j;
	char            *str;
	int             sym;
	int             comm;

	i = 0;
	j = 0;
	str = bler->line;
	sym = bler->sym;
	while (args[i])
	{
		sym = ft_strstr(str + sym, args[i]) - str + ft_strlen(args[i]);
		if (args[i + 1] != NULL)
		{
			j = ft_strstr(str + sym, args[i + 1]) - str;
			comm = 0;
			while (sym < j)
				if (str[sym++] == SEPARATOR_CHAR)
					comm++;
			if (comm > 1 || comm < 1)
				error_printf(bler, ERROR_SEVERAL_COMMAS, bler->line);
		}
		i++;
	}
	bler->sym = sym;
}

void                free_split(char **args)
{
	int             i;

	i = 0;
	while (args[i])
		free(args[i++]);
	free(args);
}

/*
 * Here we are doing several checking.
 * At first checking number of arguments. If there more 3 arguments - it's wrong player.
 * In CHECK TYPE we are checking type of arguments which should have operations.
 *
*/

void                check_arg_count_type(t_asm *bler, t_operation *oper)
{
	int             num;
	t_argument      *argm;

	num = 0;
	argm = oper->args;
	while (argm != NULL)
	{
		if (num > 3)
			error_printf(bler, ERROR_WRONG_COUNT_ARGS, bler->line);
			// op_code = номер операции; num = номер аргумента; type = тип аргумента
		if(CHECK_TYPE(oper->op_code, num,argm->type) == 0)
			error_printf(bler, ERROR_TYPE_OF_OPER, bler->line);
		argm = argm->next;
		num++;
	}
	if (CHECK_ARGS_COUNT(oper->op_code) != num)
		error_printf(bler, ERROR_WRONG_COUNT_ARGS, bler->line);
}

/*
 * В началае все аргументы выводим в сплит отдельными строками в массиве.
 * Далее очищаем каждую строку от лишних пробелов и табуляций, если они есть.
 * Setting arguments to our structure t_operation.
 * it's error.
 * Freeing memory of split in free_split.
 * Checking count of arguments in operation.
 */

void                parse_args(t_asm *bler, t_operation *oper)
{
	char **arguments;
	int i;

	i = 0;
	arguments = ft_strsplit(bler->line + bler->sym, SEPARATOR_CHAR);
	if (!(arguments))
		error_printf(bler, ERROR_ARGS, bler->line);
	while (arguments[i] != NULL)
	{
		arguments[i] = clean_voids(arguments[i]);
		i++;
	}
	set_args(bler, oper, arguments);
	free_split(arguments);
	bler->line ? ft_strdel(&bler->line) : 0;
}
