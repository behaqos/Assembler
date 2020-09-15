/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_instruments.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpenney <dpenney@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 06:21:48 by dpenney           #+#    #+#             */
/*   Updated: 2020/09/15 06:44:55 by dpenney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

/*
 * At first checking all symbols in string. If string have # or ;
 * and it's not end of line - this is comment string.
 */
void            pass_comments(char *str)
{
    int         i;
	int         len;

    i = 0;
    len = ft_strlen(str);
    while (str[i] &&
		    (str[i] != ';' &&
           str[i] != '#'))
        i++;
    if (str[i] != '\0')
    {
	    while (i < len)
	    {
		    str[i] = '\0';
		    i++;
	    }
    }
}

void            pass_voids(t_asm *bler)
{
    while (bler->line[bler->sym] && (bler->line[bler->sym] == ' '
                || bler->line[bler->sym] == '\t'))
        bler->sym++;
}

t_operation     *init_op_list(t_asm *bler)
{
    t_operation *tmp;
    t_operation *new_op;

    tmp = bler->oper;
    while (tmp && tmp->next)
        tmp = tmp->next;
    if (!(new_op = (t_operation *)ft_memalloc(sizeof(t_operation))))
        error_printf(bler, "Error: Program did not allocate memory\n", NULL);
    if (!bler->oper)
	{
    	bler->oper = new_op;
	}
    else
        tmp->next = new_op;
    return (new_op);
}

void            end_line_validation(t_asm *bler)
{
	int 		i;

	i = 0;
	if (bler->line != NULL)
		free(bler->line);
	bler->line = ft_strnew(bler->line_len);
	lseek(bler->fd, -bler->line_len, SEEK_CUR);
	read(bler->fd, &bler->line, bler->line_len);
	bler->sym = 0;
	if (!check_op(bler) && !check_label(bler))
		error_printf(bler, "TEST", NULL);
	else
	{
		i = (int)ft_strlen(bler->line) - 1;
		if (bler->line[i] == '\n')
			error_printf(bler, "TEST2", NULL);
	}
}

/*
 * Сначала проверяем нашлись ли команда либо метка.
 * Если да, то сначала создаём переменную для хранения операций.
 * Далее добавляем метку. Если её нет, то ничего страшного, внутри проводится
 * ещё одна проверка.
 * Далее проверяем на операции.
 * Если строка ещё есть, то пропускаем пробелы и табуляции.
 * Если есть что-то ещё в строке, то вывод ошибки.
 */

void            parse_lbl_op(t_asm *bler)
{
    t_operation *oper;
    char        *str;

    if (check_label(bler) ||
            check_op(bler))
    {
        oper = init_op_list(bler);
        add_lbls(bler, oper);
        add_op(bler, oper);
    }
    if (bler->line != NULL)
    {
    	//FIXME вот здесь нужно проверять на присутствие лишних символов.
    	pass_voids(bler);
	    str = bler->line + bler->sym;
    	if (*str != '\0')
		    error_printf(bler, ERROR_CONTANT, bler->line);
    }
}

void            check_end_line(t_asm *bler)
{
	int         lpos;
	char        sym;
	int         fd_res;

	fd_res = -42;
	lpos = 0;
	lpos = lseek(bler->fd, -1, SEEK_END);
	fd_res = read(bler->fd, &sym, 1);
	if (sym == '\n')
		return ;
	else
		error_printf(bler, ERROR_END_LINE, NULL);
}

void            parse_instructions(t_asm *bler)
{
    while (get_next_line(bler->fd, &bler->line) > FALSE)
    {
        bler->line_len = ft_strlen(bler->line);
        pass_comments(bler->line);
		parse_lbl_op(bler);
		bler->sym = 0;
    }
	check_end_line(bler);
}
//FIXME когда много запятых после аргументов, он их пропускает, а не выводит ошибку.