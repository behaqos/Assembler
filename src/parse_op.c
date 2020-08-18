#include "asm.h"
#define GNLINE bler->line[bler->sym]

t_op_list  op_tab[16] = {
        {"live", 1, {{{0, 1, 0}}, {{0, 0, 0}}, {{0, 0, 0}}}, 1, 0, 4},
        {"ld", 2, {{{0, 1, 1}}, {{1, 0, 0}}, {{0, 0, 0}}}, 2, 1, 4},
        {"st", 2, {{{1, 0, 0}}, {{1, 0, 1}}, {{0, 0, 0}}}, 3, 1, 4},
        {"add", 3, {{{1, 0, 0}}, {{1, 0, 0}}, {{1, 0, 0}}}, 4, 1, 4},
        {"sub", 3, {{{1, 0, 0}}, {{1, 0, 0}}, {{1, 0, 0}}}, 5, 1, 4},
        {"and", 3, {{{1, 1, 1}}, {{1, 1, 1}}, {{1, 0, 0}}}, 6, 1, 4},
        {"or", 3, {{{1, 1, 1}}, {{1, 1, 1}}, {{1, 0, 0}}}, 7, 1, 4},
        {"xor", 3, {{{1, 1, 1}}, {{1, 1, 1}}, {{1, 0, 0}}}, 8, 1, 4},
        {"zjmp", 1, {{{0, 1, 0}}, {{0, 0, 0}}, {{0, 0, 0}}}, 9, 0, 2},
        {"ldi", 3, {{{1, 1, 1}}, {{1, 1, 0}}, {{1, 0, 0}}}, 10, 1, 2},
        {"sti", 3, {{{1, 0, 0}}, {{1, 1, 1}}, {{1, 1, 0}}}, 11, 1, 2},
        {"fork", 1, {{{0, 1, 0}}, {{0, 0, 0}}, {{0, 0, 0}}}, 12, 0, 2},
        {"lld", 2, {{{0, 1, 1}}, {{1, 0, 0}}, {{0, 0, 0}}}, 13, 1, 4},
        {"lldi", 3, {{{1, 1, 1}}, {{1, 1, 0}}, {{1, 0, 0}}}, 14, 1, 2},
        {"lfork", 1, {{{0, 1, 0}}, {{0, 0, 0}}, {{0, 0, 0}}}, 15, 1, 2},
        {"aff", 1, {{{1, 0, 0}}, {{0, 0, 0}}, {{0, 0, 0}}}, 16, 1, 4}
};

/*
 * Здесь ищется операция по названию из глобальной переменной выше.
 */

int             find_oper(char *str, int len)
{
    int         i;

    i = 0;
    while (i < 16)
    {
        if (ft_strnequ(op_tab[i].name, str, len))
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

    start = 0;
    i = 0; // TODO пропускает первую операцию. Почему?
    if (check_label(bler))
        bler->line = ft_strchr(bler->line, ':') + 1;
	while (bler->line[i] == ' ' || bler->line[i] == '\t')
		i++;
    while (bler->line[i] && ft_isalpha(bler->line[i]))
        i++;
    if (i > 0 && find_oper(bler->line, i) != -1)
        return (TRUE);
    else
        return (FALSE);
}

/*
 * GNLINE is (bler->line[bler->sym])
 * Пропускаем, пока не встретим особые символы.
 * Если пропускаем символы
 */
//FIXME должен с правильной точки начать копировать имя, иначе имя теряется и не находитс имя операции.
void             parse_op(t_asm *bler, t_operation *oper)
{
    int         start;

    bler->sym = 0;
	while (GNLINE && GNLINE != '-' && GNLINE != '\t' &&
			GNLINE != ' ' && GNLINE != '%' && !ft_isdigit(GNLINE))
		bler->sym++;
	if (GNLINE != ' ' && GNLINE != '\t' &&
		GNLINE != '%' && GNLINE != '-')
		error_printf(bler, ERROR_LINE, bler->line);
	oper->name = ft_strsub(bler->line, start, bler->sym);
	oper->op_code = find_oper(oper->name, ft_strlen(oper->name));
}

/*
 * Парсим операцию.
 * Если операция нашлась, то парсим в parse_op имя операции и его код;
 * Далее парсим аргументы операциии.
 */
// FIXME неправильно записывает имя операции.
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