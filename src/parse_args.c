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

/*
 * Проходимся по аргументам. В ft_strstr передаём позицию, с которой начинаются аргументы.
 * И передаём аргумент, который запарсили в виде строки. ft_strstr должен пропустить операцию.
 */

void                check_commas(t_asm *bler, t_operation *oper, char **args)
{
	int             i;
	int             j;
	char            *str;
	int             sym;

	str = bler->line + bler->sym;
	sym = bler->sym;
	while (args[i])
	{
		sym = ft_strstr(str + sym, args[i]);
		sym = sym - str +

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
	set_args(bler, oper, args);
	print_operation(bler, oper);
//	// TODO проверка правильно расставленных запятых.
	check_commas(bler, oper, args);
//	// TODO очистика всего массива, выделенного для сплита аргументов.
//	array_map(arr, ft_strdel);
//	free(arr);
//	// TODO подсчет кол-ва аргументов
//	check_arguments(new);
}

//TODO detector
