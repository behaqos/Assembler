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

/*
** We pass flag just in order to determine if this argument is INT or CHAR *.
** 'add_argument(command, type, get_data(type, arr[i], &flag), flag)'
*/

/*
 * Функция призвана найти типы каждого аргументы
 */

static	void	foreach_arg(char **arr, t_command *command)
{
	int			i;
	char		type;
	char		flag;

	i = 0;
	/*
	 * В цикле получаем каждый запарсингованный аргумент и передаем на проверку.
	 * В type хранится тип аргумента, который найден в get_type.
	 * Если тип не найден, то возвращается константа (-1) и выводится ошибка.
	 * Далее берётся аргумент и присваивается операции.
	 * Для этого отправляем операцию, тип, взятые данные аргумента и флаг.
	 * В get_data мы отправляем флаг, чтобы знать - парсим цифру или строку.
	 */
	while (arr[i])
	{
		flag = 0;
		type = get_type(arr[i]);
		if (type == UNDEFINED_TYPE)
			ft_error(ERR_UNDEFINED_ARG);
		add_argument(command, type, get_data(type, arr[i], &flag), flag);
		i++;
	}
}

static	void	check_arguments(t_command *command)
{
	t_arg		*arg;
	int			len;

	len = 0;
	arg = command->args;
	while (arg)
	{
		// Если количество аргументов больше трёх, ошибка.
		(len > 3) ? ft_error(ERR_TOO_MUCH_ARGS) : 0;
		// Если нет g_table[i - 1].args[j].arg[(int)k], который принимает код операции, длину и типа, то огибка
		if (!ARG(command->opcode, len, arg->type))
			ft_error(ERR_NOT_COMPATIBLE_ARG);
		// инкрементируем кол-во аргументов.
		len++;
		arg = arg->next;
	}
	// Если кол-во аргументов больше, чем надо для это операции, то ошибка.
	(COUNT_ARGS(command->opcode) != len) ? ft_error(ERR_COUNT_ARGS) : 0;
}

/*
** k - it is a temporary variable that represents
** the beginning of the next argument.
** q - is is a temporary variable that represents
** quantity of ',' between two arguments.
** i - is is a temporary variable that represents
** quantity of arguments.
*/

/*
 * В цикле берём по одному аргументу, где они хранятся в строках.
 * в j - хранится итератор с количеством символов после табуляци/пробелов и имени операции,
 * где непосредственно подходит к аргументам.
 * ft_strstr находит вхождение строки. В неё подаются строка из GNL и суммируется с итераторм j, чтобы
 * пропустить пробелы/табуляции, имя операции и быть на позиции к аргументам, а второй строкой сам аргумент.
 * Когда мы доходим до точки начала аргумента, из ft_strstr приходит итератор, который показывает начало букв второй строки
 * Когда сложим строку с итератором, то перейдем сразу к аргументам.
 * В ft_strstr получаем полную линию, которая начинается с аргументов и сравниваем с аргументом.
 * Когда находим начало букв настоящего аргумента в строке, то возвращаем итератор.
 * Далее отнимаем от строки + 
 */

static	void	skip_args(char *s, char **arr, int *j)
{
	int			i;
	int			k;
	int			q;

	i = 0;
	k = 0;
	// проходимся по каждому аргументу
	while (arr[i])
	{
		*j = ft_strstr(s + *j, arr[i]) - s + ft_strlen(arr[i]);
		// если ещё есть аргумент, то
		if (arr[i + 1])
		{
			// находим его первое вхождение второй строки в первой и отнимаем от первой строки.
			k = ft_strstr(s + *j, arr[i + 1]) - s;
			q = 0;
			// в цикле пока j, который хранит значение цифры, где остановился, найдя первый аргумент
			// меньше вхождения второй строки, который хранится в k
			/*
			 * Так мы двигаемся от первого аргумента ко второму.
			 * Если есть запятая, то q увеличиваем, идём дальше.
			 * Если запятая есть одна, то отлично. Если их больше, то ошибка.
			 * Так мы сравниваем 1 и второй аргумент, а дальше 2 и 3, если 3-ий есть.
			 */
			while (*j < k)
			{
				(s[*j] == SEPARATOR_CHAR) ? q++ : 0;
				(*j)++;
			}
			(q != 1) ? ft_error(ERR_TOO_MUCH_SEPARATORS) : 0;
		}
		i++;
	}
}

static	void	array_map(char **arr, void (*f)(char **))
{
	int			i;

	i = 0;
	while (arr[i])
	{
		(*f)(&arr[i]);
		i++;
	}
}

void            print_operation(t_command *opera) {
	t_command *oper;
	int i = 1;
	oper = opera;
	t_arg *args;

	args = oper->args;
	printf("Operation name: %s\nLabel: %s\n\n", oper->name, (char *) oper->labels->content);
	while (args) {
		printf("ARG #%d ", i);
		if (args->flag == 1)
			printf("Value: %d\n", args->num_value);
		else
			printf("Value: %s\n", args->str_value);
		//TYPES
		if (args->type == T_REG)
			printf("                Type:%s", "T_REG");
		else if (args->type == T_DIR)
			printf("                Type:%s", "T_DIR");
		else if (args->type == T_IND)
			printf("                Type:%s", "T_IND");
		args = args->next;
		i++;
		printf("\n");
	}
}

void			get_arguments(t_asm *asmb, t_command *new, int *j)
{
	char		**arr;

	if (!(arr = ft_strsplit(asmb->line + *j, SEPARATOR_CHAR)))
		ft_error(ERR_MALLOC);
	// очищает от пробелов и табуляц;
	array_map(arr, ft_strtrim);
	// добавляет все аргументы в операцию.
	foreach_arg(arr, new);
//	print_operation(new);
	// TODO проверка правильно расставленных запятых.
	skip_args(asmb->line, arr, j);
	// TODO очистика всего массива
	array_map(arr, ft_strdel);
	free(arr);
	// TODO подсчет кол-ва аргументов
	check_arguments(new);
}
