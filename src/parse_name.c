/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_name.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpenney <dpenney@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 06:21:55 by dpenney           #+#    #+#             */
/*   Updated: 2020/09/15 06:44:56 by dpenney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void record(t_asm *bler, int code, int *c, char s)
{
	if (code < 2)
		bler->name[*c] = s;
	else
		bler->comment[*c] = s;
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
				record(bler, *code, c, bler->line[i]);
		}
		if (bler->line[i] == '\"' && *code % 2 == 0 && *c == 0)
			*code = *code + 1;
		i++;
	}
	return (*c);
}

//FIXME Почему флаг 4?
/*
 * В данной фукции мы парсим имя и комментарий чемпиона.
 * С помощью хитрой флаговой системы мы получаем данные
 * и валидируем их.
 */
//FIXME Если комментарий перед именем и комментом, то выводится ошибка! Игрок Car из vm_champs/champs
void            parse_name_comm(t_asm *bler)
{
	int flag;
	int len;

	flag = 0;
	bler->name = ft_strnew(PROG_NAME_LENGTH);
	bler->comment = ft_strnew(COMMENT_LENGTH);
	while (flag < 4 && get_next_line(bler->fd, &bler->line) > 0)
	{
		if (flag == 0 || flag == 2)
			len = 0;
		else if (flag == 1 || flag == 3)
			record(bler, flag, &len, '\n');
		if (flag == 0 && !ft_strncmp(".name", bler->line, 5))
			len = write_name(bler, &len, &flag, 5);
		else if (flag == 2 && !ft_strncmp(".comment", bler->line, 8))
			len = write_name(bler, &len, &flag, 8);
		else
			len = write_name(bler, &len, &flag, 0);
		free(bler->line);
	}
	flag != 4 ? error_printf(bler, ERROR_NOT_FOUND_NM_CM, NULL) : 0;
	pass_voids(bler);
	if ((check_label(bler) == FALSE && check_op(bler) == FALSE) && ft_isalnum(bler->line[bler->sym]))
		error_printf(bler, ERROR_UNKNOWN_TEXT, bler->line);
}
