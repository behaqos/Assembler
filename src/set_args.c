/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpenney <dpenney@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 06:22:07 by dpenney           #+#    #+#             */
/*   Updated: 2020/09/15 06:44:59 by dpenney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

/*
 * Обычно T_REG имеет переменные максимум 16. Поэтому здесь проверяется именно это.
 */

int                 check_reg_num(char *str)
{
	int             i;
	unsigned        res;

	i = 0;
	res = 0;
	if (str && *str)
	{
		if (*str == '-' || *str == '+')
			str++;
		while (ft_isdigit(*str))
		{
			if (res > MAX_UINT || (res == NEARLY_MAX_UINT &&
			                       *str >= '5'))
				return (0);
			res = res * 10 + *str - 48;
			i++;
			str++;
		}
		if (str[i] == '\0' && 0 < i)
			return (TRUE);
		else
			return (FALSE);
	}
	return (FALSE);
}

/*
 * Если есть % и :, значит дальше будет метка,
 * метка парсится отдельно.
 * Дальше смотрим - если дальше идут до конца цифры, то парсим только цифры.
 */

int                 check_t_dir(char *str)
{
	int             i;

	i = 0;
	if (str[i] && str[i] == '%')
	{
		i++;
		if (str[i] == ':')
			return (TRUE);
		else if (str[i] != '\0')
		{
			if (str[i] == '-')
				i++;
			while (ft_isdigit(str[i]))
				i++;
			if (str[i] == '\0')
				return (TRUE);
		}
	}
	return (FALSE);
}

/*
 * If string begin with ':' that mean label;
 * Else if have digits, it's IND
 */

int                 check_t_ind(char *str)
{
	int             i;

	i = 0;
	if (str[i] == ':')
	{
		i++;
		if (!ft_strchr(str, ':'))
			return (TRUE);
	}
	else if (str[i] != '\0')
	{
		if (str[i] == '-')
			i++;
		while (ft_isdigit(str[i]))
			i++;
		if (str[i] == '\0')
			return (TRUE);
	}
	return (FALSE);
}

/*
 * Checking arguments by 3 types - T_REG, T_DIR, T_IND(ID);
 */

int                 search_types(char *str)
{
	int             arg_num;

	arg_num = 0;
	if (*str == 'r')
	{
		str++;
//		if (*str == '-' || *str == '+' || !check_reg_num(str))
		if (*str == '-' || *str == '+')
				return (WRONG_TYPE);
		arg_num = ft_atoi(str);
		if (arg_num <= REG_NUMBER && arg_num > 0)
			return (T_REG);
		else
			return (WRONG_TYPE);
	}
	else if (check_t_dir(str))
		return (T_DIR);
	else if (check_t_ind(str))
		return (T_IND);
	return (WRONG_TYPE);
}

/*
 * Находим тип. Если тип не найден, то вывод ошибки.
 * Когда тип найден, то парсим эти данные в структуру данных t_args.
 * А дальше фигачим в операцию всю инфу.
 */

void				set_op_size(t_operation *opera)
{
	int len;
	t_argument *args;

	len = 1;
	args = opera->args;
	if (opera->code_type_arg)
		len++;
	while (args)
	{
		len += args->args_size;
		args = args->next;
	}
	opera->op_size = len;
}


void                set_args(t_asm *bler, t_operation *oper, char **args)
{
	int             detector;
	int             i;
	int             type;
	void            *arg;


	i = 0;
	check_commas(bler, args);
	while (args[i] != NULL)
	{
		detector = 0;
		if ((type = search_types(args[i])) == -1)
			error_printf(bler, ERROR_ARGS_TYPE, bler->line);
		arg = parse_data(args[i], type, &detector);
		join_argument(oper, arg, type, detector);
		i++;
	}
	check_arg_count_type(bler, oper);
	set_op_size(oper);
	print_operation(bler, oper);
}