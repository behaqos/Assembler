#include "asm.h"
//TODO продебажить правильность функци.
/*
 * This function cleaning strings of arguments of tabs and spaces.
 */
void           		clean_voids(char **str)
{
	int             x;
	int             y;
    char            *res;

	if (str == NULL || *str == NULL)
		return ;
	x = 0;
	while (*str[x] && (*str[x] == ' ' ||
			*str[x] == '\n' || *str[x] == '\t'))
		x++;
	y = ft_strlen(*str) - 1;
	while (x < y && ((*str)[y] == ' ' ||
	        (*str)[y] == '\n' ||
            (*str)[y] == '\t'))
	    y--;
	y = y - x + 1;
	if (!(res = ft_strsub(*str, x, y)))
	    error_printf(NULL, ERROR_ALLOCATE, NULL);
	ft_strdel(str);
	*str = res;
}

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
//TODO можем изменить возвращение на int вместо char?
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

 // TODO разбей эту функцию
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

void                join_arg_to_oper(t_operation *oper, t_argument *new_arg)
{
	t_argument      *tmp;

	tmp = oper->args;
	if (!tmp)
	{
		oper->args = new_arg;
		return ;
	}
	while (tmp && tmp->next)
		tmp = tmp->next;
	tmp->next = new_arg;
}

/*
 * Выделяем место для арумента в операции, обнуляем все значения.
 * Далее добалвяем новый аргумент в операцию.
 * Дальше смотрим - если аргумент строка - то добавляем в переменную строки.
 * И указываем флаг STRING_VAL в аргументе.
 * Если строка, то указывается во флаге 2. Если цифры, то указ-ся 1.
 * detector нужен чтобы понимать - это строка или цифры.
 */


//TODO разбить функцию, плавлено

void                join_argument(t_operation *oper, void *arg, int type, int detector)
{
	t_argument     *new_arg;

	if (!(new_arg = (t_argument *)ft_memalloc(sizeof(t_argument))))
		error_printf(NULL, ERROR_ALLOCATE, NULL);
	new_arg->type = type;
	join_arg_to_oper(oper, new_arg);
	if (detector == STRING_VAL)
	{
		new_arg->str_val = (char *)arg; // TODO почему указвает желтую полосу?
		new_arg->detector = STRING_VAL;
	}
	else if (detector == NUM_VAL)
	{
		new_arg->num_val = ((int *)arg)[0]; // неправильно записывается.
		free(arg); // FIXME почему не очищается arg?
	}
	// TODO set_arg_size()
}
/*
 * Находим тип. Если тип не найден, то вывод ошибки.
 * Когда тип найден, то парсим эти данные в структуру данных t_args.
 * А дальше фигачим в операцию всю инфу.
 */

void                set_types(t_asm *bler, t_operation *oper, char **args)
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
}

void                parse_args(t_asm *bler, t_operation *oper)
{
	char **args;
	int i;

	i = 0;
	args = ft_strsplit(bler->line + bler->sym, SEPARATOR_CHAR);
	if (!(args))
		error_printf(bler, ERROR_ARGS, bler->line);
	while (args[i])
		clean_voids(&args[i++]);
	set_types(bler, oper, args);
}


