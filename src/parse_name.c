#include "../includes/asm.h"

/*
 * Проверка комментария после закрытия кавычек например .name "name" # Данный коммент проверяется на валидность
 */

int         check_comment(const char* str)
{
	int i;

	i = 0;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i] == COMMENT_CHAR || str[i] == ALT_COMMENT_CHAR || str[i] == '\0')
		return (0);
	return (1);
}

/*
 * Пропуск пробелов и табов
 */

int         skip_delimiters(char *str, int i)
{
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	return(i);
}

/*
 * Функция которая переписывает с line в name или comment
 */

void record(t_asm *bler, int code, int *c, int i)
{
	if (code < 2)
		bler->name[*c] = bler->line[i];
	else
		bler->comment[*c] = bler->line[i];
	*c = *c + 1;
	if (code < 2 && *c > PROG_NAME_LENGTH)
		error_printf(bler, ERROR_NAME_LEN, bler->line);
	else if (code > 1 && *c > COMMENT_LENGTH)
		error_printf(bler, ERROR_COMM_LEN, bler->line);
}

/*
 * Основная функция, где реализованна вся логика, проверки и обработка строки.
 */

int                     write_name(t_asm *bler, int *c ,int *code, int i)
{
	*code % 2 == 0 ? i = skip_delimiters(bler->line, i) : i;
	if (bler->line[i] != '\"' && *code % 2 == 0)
		error_printf(bler, ERROR_WOKS_NM_CM, bler->line);
	while (bler->line[i] != '\0' && *code < 4)
	{
		if (*code % 2 == 1)
		{
			if (bler->line[i] == '\"')
			{
				i++;
				if (check_comment(&bler->line[i]))
					error_printf(bler, ERROR_WOKS_NM_CM, bler->line);
				*code = *code + 1;
			}
			else
				record(bler, *code, c, i);
		}
		if (bler->line[i] == '\"' && *code % 2 == 0 && *c == 0)
			*code = *code + 1;
		i++;
	}
	return (*c);
}

//FIXME Почему флаг 4? Замени константой

void            parse_name_comm(t_asm *bler) {
	int flag;
	int len;

	flag = 0;
	bler->name = ft_strnew(PROG_NAME_LENGTH);
	bler->comment = ft_strnew(COMMENT_LENGTH);
	while (get_next_line(bler->fd, &bler->line) > 0 && flag < 4) {
		if (flag == 0 || flag == 2)
			len = 0;
		if (flag == 0 && !ft_strncmp(".name", bler->line, 5))
			len = write_name(bler, &len, &flag, 5);
		else if (flag == 2 && !ft_strncmp(".comment", bler->line, 8))
			len = write_name(bler, &len, &flag, 8);
		else
			len = write_name(bler, &len, &flag, 0);
	}
	if (flag != 4)
		error_printf(bler, ERROR_NOT_FOUND_NM_CM, NULL);
	if (check_label(bler) == FALSE || check_op(bler) == FALSE)
		error_printf(bler, ERROR_UNKNOWN_TEXT, bler->line);
}