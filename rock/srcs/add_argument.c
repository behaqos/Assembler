/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkiselev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/16 16:39:55 by tkiselev          #+#    #+#             */
/*   Updated: 2018/07/16 16:39:57 by tkiselev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

t_arg				*push_new_arg(t_arg **args)
{
	t_arg			*new;
	t_arg			*tmp;

	if (!(new = (t_arg *)malloc(sizeof(t_arg))))
		ft_error(ERR_MALLOC);
	new->str_value = NULL;
	new->num_value = 0;
	new->arg_size = 0;
	new->type = 0;
	new->flag = 0;
	new->next = NULL;
	tmp = *args;
	while (tmp && tmp->next)
		tmp = tmp->next;
	if (!tmp)
		*args = new;
	else
		tmp->next = new;
	return (new);
}

/*
 * Здесь записываем всё исходя из  констант;
 * Однако если это DIR, то
 */

static	char		get_arg_size(char opcode, char type)
{
	if (type == T_REG)
		return (T_REG_SIZE);
	else if (type == T_IND)
		return (T_IND_SIZE);
	else
	{
	    //TODO Разберись что здесь происходит.
		if (LABEL_SIZE(opcode) == 4)
			return (4);
		else
			return (T_DIR_SIZE);
	}
}

/*
** At the beginning we have ':'.
** 'return ((void*)my_strsub(str + 1, 0, ft_strlen(str) - 1))'
*/

/*
 * В этой функции получаем значения аргументов.
 * Если это тип T_REG, то флагу присваиваем код этого типа.
 * Далее выделяем память под положительное целое число, куда присваиваем значение.
 * Возвращаем числовое значение.
 * Установлено void значение, т.к. может быть как строка, так и цифры.
 */

void				*get_data(char type, char *str, char *flag)
{
	unsigned int	*num_val;

	if (type == T_REG)
	{
		*flag = UINT_VAL; // указываем, что это цифры.
		num_val = (unsigned int *)malloc(sizeof(unsigned int));
		*num_val = ft_atoi(str + 1);
		return ((void*)num_val);
	}
	else
	{
	/*
	 * Если же там символ %, то идём дальше.
	 * Если дальше двоеточие, тогда обрезаем эту часть аргумента и воз-ем.
	 * А флагу указываем, что это строка, чтобы правильно получать.
	 */
		(*str == '%') ? str++ : 0;
		if (*str == ':')
		{
			*flag = STRING_VAL;
			return ((void*)my_strsub(str + 1, 0, ft_strlen(str) - 1));
		}
		else
		{
		    /*
		     * Иначе записываем как цифру, при этом указывая флаг, что там цифра.
		     */
			*flag = UINT_VAL;
			num_val = (unsigned int *)malloc(sizeof(unsigned int));
			*num_val = ft_atoi(str);
			return ((void*)num_val);
		}
	}
}

/*
** We must return to this variable
** because it's just a pointer to a label
** which we cannot have on this stage.
** 'new->flag = 1'
*/

/*
 * Здесь присваиваем значения в операцию.
 * type - хранит тип аргумента
 * data - значение аргумента;
 * flag - указывает что мы будем парсить.
 * В зависимости от флага - парсим
 *
 * Структура данных t_arg хранит в себе все данные
 * аргументов в зависимости от их характеристик.
 * Сначала выделяется память для аргументов в структуре данных операций.
 * Дальше уже начинаем присваиваение. Если это строка, то записываем строку
 * Указываем флагом единицу.
 * Если это цифра, то указываем записываем  цифру
 * и очищаем цифру.
 * В тип записываем тип.
 * Размер аргумента находим в функци get_arg_size и записываем тоже в структуру.
 * (opcode);
 */

void				add_argument(t_command *command, char type,
					void *data, char flag)
{
	t_arg			*new;

	new = push_new_arg(&command->args);
	if (flag == STRING_VAL)
	{
		new->str_value = (char *)data;
		new->flag = 1;
	}
	else if (flag == UINT_VAL)
	{
		new->num_value = ((int *)data)[0]; //TODO почему ячейку передает?
		free(data);
	}
	new->type = type;
	new->arg_size = get_arg_size(command->opcode, type);
}
