#include "asm.h"

/*
 * в этой функции в зависимости от типа аргумента парсим данные.
 * Если это T_REG, то парсим цифру от r переменной.
 * Если аргумент начинается с %, а дальше : - значит это метка, копируем её как строку.
 * Если просто %, то парсим самое значение как цифру.
 * TODO разбей эту функцию
 */

void                *parse_data(char *arg, int type, int *detector)
{
	unsigned        *value_num;
	int             i;
	char            *str;

	i = 0;
	if (type == T_REG)
	{
		*detector = NUM_VAL;
		if (!(value_num = (unsigned *)ft_memalloc(sizeof(unsigned))))
			return (NULL);
		*value_num = ft_atoi(arg + 1);
		return ((void *)value_num);
	}
	else
	{
		if (arg[i] == '%' && arg[i + 1] == ':')
		{
			i += 2;
			*detector = STRING_VAL;
			if (!(str = ft_strsub(arg + i, 0, ft_strlen(arg) - 1)))
				return (NULL);
			return ((void *) str);
		}
		else if (arg[i] == '%')
		{
			*detector = NUM_VAL;
			if (!(value_num = (unsigned *)ft_memalloc(sizeof(unsigned))))
				return (NULL);
			*value_num = (unsigned)ft_atoi(arg + i); // FIXME записывается NULL и обнуляет переменную. А должен записать ноль.
			return ((void *)value_num);
		}
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
 * Дальше смотрим - если дальше идут до конца цифры, то отлично.
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
 *
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
	int             i;
	int             arg_num;

	arg_num = 0;
	i = 0;
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
	while (args[i] != NULL)
	{
		detector = 0;
		if ((type = search_types(args[i])) == -1)
			error_printf(bler, ERROR_ARGS_TYPE, bler->line);
		arg = parse_data(args[i], type, &detector);
		join_argument(oper, arg, type, detector);
		i++;
	}
	check_commas(bler, oper, args);
	//print_operation(bler, oper);
	check_arg_count_type(bler, oper);
}