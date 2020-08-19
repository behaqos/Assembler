#include "asm.h"
#define GNLINE bler->line[bler->sym]

/*
 * Здесь ищется операция по названию из глобальной переменной выше.
 */

int             find_oper(char *str, int len)
{
    int         i;

    i = 0;
    while (i < 16)
    {
        if (ft_strnequ(g_op_tab[i].name, str, len))
            return (i);
        i++;
    }
    return (-1);
}

/*
 * Сначала проверяется есть ли метка. Если метка есть, то до двоеточия всё пропускается
 * Далее проверяем являются ли следующие символы буквами, что говорит нам об операциях.
 * Далее ищём операцию в find_oper, в котором в цикле перебираются названия всех операций.
 * Если операция найдена, то функция отвечает TRUE, иначе FALSE.
 */

int             check_op(t_asm *bler)
{
    int         i;
    int         start;
	char        *str;
    start = 0;
    i = 0;

    str = bler->line;
    if (check_label(bler))
        str = ft_strchr(bler->line, ':') + 1;
	while (*str == ' ' || *str == '\t')
		str++;
    while (str[i] && ft_isalpha(str[i]))
        i++;
    if (i > 0 && find_oper(str, i) != -1)
        return (TRUE);
    else
        return (FALSE);
}

/*
 * GNLINE is (bler->line[bler->sym])
 * Пропускаем все пробелы и табуляции в pass_voids
 * В цикле проверяем, что идут буквы. Если да, то считаем их кол-во.
 * Далее смотрим, что после букв стоит табуляция или пробел, которые разделяют
 * операции от аргументов. А если их нет, то выводим ошибку.
 * Дальше отправляем в ft_strsub, чтобы получить отрезок от основной строки.
 */

void             parse_op(t_asm *bler, t_operation *oper)
{
    int         start;
	int         len;

	len = 0;
    start = 0;
    bler->sym = 0;
    pass_voids(bler);
    start = bler->sym;
	while (GNLINE && GNLINE != '-' && GNLINE != '\t' &&
			GNLINE != ' ' && GNLINE != '%' && !ft_isdigit(GNLINE))
	{
		len++;
		bler->sym++;
	}
	if (GNLINE != ' ' && GNLINE != '\t' &&
		GNLINE != '%' && GNLINE != '-')
		error_printf(bler, ERROR_LINE, bler->line);
	oper->name = ft_strsub(bler->line, start, len);
	oper->op_code = find_oper(oper->name, ft_strlen(oper->name));
}

/*
 * Парсим операцию.
 * Если операция нашлась, то парсим в parse_op имя операции и его код;
 * Далее парсим аргументы операциии.
 */

void             add_op(t_asm *bler, t_operation *oper)
{
    if (check_op(bler))
    {
        parse_op(bler, oper);
        parse_args(bler, oper);
    }
    else
        return ;
}