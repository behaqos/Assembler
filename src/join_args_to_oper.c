#include "asm.h"

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
 * Здесь проверяются размеры типов.
 * У T_REG обычно 1 байт.
 * T_IND - 2 байта.
 * А у T_DIR зависит от операции. У некоторых операций 2 байта, у других - 4 байта.
 */

int         set_args_size(t_operation *oper, int type)
{
	int     i;

	i = 0;
	if (type == T_DIR)
	{
		while (i <= 16)
		{
			if (ft_strcmp(g_op_tab[i].name, oper->name) == 0)
				return (g_op_tab[i].lbl_size);
			i++;
		}
	}
	else if (type == T_IND)
		return (SIZE_T_IND);
	else if (type == T_REG)
		return (SIZE_T_REG);
	else
		error_printf(NULL, ERROR_TYPE_SIZE, NULL);
}

/*
 * Выделяем место для арумента в операции, обнуляем все значения.
 * Далее добалвяем новый аргумент в операцию.
 * Дальше смотрим - если аргумент строка - то добавляем в переменную строки.
 * И указываем флаг STRING_VAL в аргументе.
 * Если строка, то указывается во флаге 2. Если цифры, то указ-ся 1.
 * detector нужен чтобы понимать - это строка или цифры.
 * type указывает на тип аргумента - T_REG, T_DIR, T_IND.
 */


//TODO разбить функцию, плавлено

void                join_argument(t_operation *oper, void *arg, int type, int detector)
{
	t_argument     *new_arg;

	if (!(new_arg = (t_argument *)ft_memalloc(sizeof(t_argument))))
		error_printf(NULL, ERROR_ALLOCATE, NULL);
	new_arg->type = type;
	join_arg_to_oper(oper, new_arg);
	new_arg->detector = detector;
	if (detector == STRING_VAL)
	{
		new_arg->str_val = (char *)arg;
		new_arg->detector = STRING_VAL;
	}
	else if (detector == NUM_VAL)
	{
		new_arg->num_val = ((int *)arg)[0]; // неправильно записывается.
		free(arg); // FIXME почему не очищается arg?
	}
	new_arg->type = type;
	new_arg->args_size = set_args_size(oper, type);
}