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
	unsigned        *value_num;
	int             i;
	char            *str;

	i = 0;
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

/*
 * Обычно T_REG имеет переменные максимум 16. Поэтому здесь проверяется именно это.
 */

int                 check_reg_num(char *str)
{
	int             i;
	int             res;


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
		if (*str == '-' || *str == '+' ||
		    !check_reg_num(str))
			return (FALSE);
		arg_num = ft_atoi(str);
		if (arg_num <= REG_NUMBER && arg_num >= 0)
			return (T_REG);
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

void                set_args(t_asm *bler, t_operation *oper, char **args)
{
	int             detector;
	int             i;
	int             type;
	void            *arg;


	i = 0;
	check_commas(bler, oper, args);
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
	print_operation(bler, oper);
}